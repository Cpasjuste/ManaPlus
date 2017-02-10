/*
 * This code provides a glue layer between PhysicsFS and Simple Directmedia
 *  Layer's (SDL) RWops i/o abstraction.
 *
 * License: this code is public domain. I make no warranty that it is useful,
 *  correct, harmless, or environmentally safe.
 *
 * This particular file may be used however you like, including copying it
 *  verbatim into a closed-source project, exploiting it commercially, and
 *  removing any trace of my name from the source (although I hope you won't
 *  do that). I welcome enhancements and corrections to this file, but I do
 *  not require you to send me patches if you make changes. This code has
 *  NO WARRANTY.
 *
 * Unless otherwise stated, the rest of PhysicsFS falls under the zlib license.
 *  Please see LICENSE.txt in the root of the source tree.
 *
 * SDL falls under the LGPL license. You can get SDL at http://www.libsdl.org/
 *
 *  This file was written by Ryan C. Gordon. (icculus@icculus.org).
 *
 *  Copyright (C) 2012-2017  The ManaPlus Developers
 */

#include "utils/virtfsrwops.h"

#include "logger.h"

#include "utils/fuzzer.h"
#include "utils/physfsmemoryobject.h"

#include <map>

#include "debug.h"

#ifdef USE_SDL2
#define PHYSFSINT int64_t
#define PHYSFSSIZE size_t
#else  // USE_SDL2
#define PHYSFSINT int32_t
#define PHYSFSSIZE int
#endif  // USE_SDL2

#ifdef DUMP_LEAKED_RESOURCES
static int openedRWops = 0;
#endif  // DUMP_LEAKED_RESOURCES

#ifdef DEBUG_PHYSFS
namespace
{
    std::map<void*, PHYSFSMemoryObject*> mRWops;
}  // namespace

static SDL_RWops *addDebugRWops(SDL_RWops *restrict const rwops,
                                const char *restrict const name,
                                const char *restrict const file,
                                const unsigned line)
{
    if (!rwops)
        return nullptr;

    mRWops[rwops] = new PHYSFSMemoryObject(name, file, line);
    return rwops;
}

static void deleteDebugRWops(SDL_RWops *const rwops)
{
    if (!rwops)
        return;

    std::map<void*, PHYSFSMemoryObject*>::iterator it = mRWops.find(rwops);
    if (it == mRWops.end())
    {
        logger->log("bad RWops delete: %p", static_cast<void*>(rwops));
    }
    else
    {
        PHYSFSMemoryObject *const obj = (*it).second;
        if (obj)
        {
            mRWops.erase(rwops);
            delete obj;
        }
    }
}

void VirtFs::reportLeaks()
{
    if (!mRWops.empty())
    {
        logger->log("RWops leaks detected");
        std::map<void*, PHYSFSMemoryObject*>::iterator it = mRWops.begin();
        const std::map<void*, PHYSFSMemoryObject*>::iterator
            it_end = mRWops.end();
        for (; it != it_end; ++it)
        {
            PHYSFSMemoryObject *obj = (*it).second;
            if (obj)
            {
                logger->log("file: %s at %s", obj->mName.c_str(),
                    obj->mAddFile.c_str());
                delete obj;
            }
        }
        mRWops.clear();
    }
}
#endif  // DEBUG_PHYSFS

static PHYSFSINT physfsrwops_seek(SDL_RWops *const rw,
                                  const PHYSFSINT offset,
                                  const int whence)
{
    if (!rw)
        return -1;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    PHYSFSINT pos = 0;

    if (whence == SEEK_SET)
    {
        pos = offset;
    } /* if */
    else if (whence == SEEK_CUR)
    {
        const int64_t current = VirtFs::tell(handle);
        if (current == -1)
        {
            logger->assertLog(
                "physfsrwops_seek: Can't find position in file: %s",
                VirtFs::getLastError());
            return -1;
        } /* if */

        pos = CAST_S32(current);
        if (static_cast<int64_t>(pos) != current)
        {
            logger->assertLog("physfsrwops_seek: "
                "Can't fit current file position in an int!");
            return -1;
        } /* if */

        if (offset == 0)  /* this is a "tell" call. We're done. */
            return pos;

        pos += offset;
    } /* else if */
    else if (whence == SEEK_END)
    {
        const int64_t len = VirtFs::fileLength(handle);
        if (len == -1)
        {
            logger->assertLog("physfsrwops_seek:Can't find end of file: %s",
                VirtFs::getLastError());
            return -1;
        } /* if */

        pos = static_cast<PHYSFSINT>(len);
        if (static_cast<int64_t>(pos) != len)
        {
            logger->assertLog("physfsrwops_seek: "
                "Can't fit end-of-file position in an int!");
            return -1;
        } /* if */

        pos += offset;
    } /* else if */
    else
    {
        logger->assertLog("physfsrwops_seek: Invalid 'whence' parameter.");
        return -1;
    } /* else */

    if (pos < 0)
    {
        logger->assertLog("physfsrwops_seek: "
            "Attempt to seek past start of file.");
        return -1;
    } /* if */

    if (!VirtFs::seek(handle, static_cast<uint64_t>(pos)))
    {
        logger->assertLog("physfsrwops_seek: seek error: %s",
            VirtFs::getLastError());
        return -1;
    } /* if */

    return pos;
} /* physfsrwops_seek */

static PHYSFSSIZE physfsrwops_read(SDL_RWops *const rw,
                                   void *ptr,
                                   const PHYSFSSIZE size,
                                   const PHYSFSSIZE maxnum)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    const int64_t rc = VirtFs::read(handle, ptr,
        CAST_U32(size),
        CAST_U32(maxnum));
    if (rc != static_cast<int64_t>(maxnum))
    {
        if (!VirtFs::eof(handle)) /* not EOF? Must be an error. */
        {
            logger->assertLog("physfsrwops_seek: read error: %s",
                PHYSFS_getLastError());
        }
    } /* if */

    return CAST_S32(rc);
} /* physfsrwops_read */

static PHYSFSSIZE physfsrwops_write(SDL_RWops *const rw, const void *ptr,
                                    const PHYSFSSIZE size,
                                    const PHYSFSSIZE num)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    const int64_t rc = VirtFs::write(handle, ptr,
        CAST_U32(size),
        CAST_U32(num));
    if (rc != static_cast<int64_t>(num))
    {
        logger->assertLog("physfsrwops_seek: write error: %s",
            PHYSFS_getLastError());
    }

    return CAST_S32(rc);
} /* physfsrwops_write */

static int physfsrwops_close(SDL_RWops *const rw)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file*>(
        rw->hidden.unknown.data1);
    if (!VirtFs::close(handle))
    {
        logger->assertLog("physfsrwops_seek: close error: %s",
            VirtFs::getLastError());
        return -1;
    } /* if */

    SDL_FreeRW(rw);
#ifdef DUMP_LEAKED_RESOURCES
    if (openedRWops <= 0)
        logger->assertLog("physfsrwops_seek: closing already closed RWops");
    openedRWops --;
#endif  // DUMP_LEAKED_RESOURCES
#ifdef DEBUG_PHYSFS
    deleteDebugRWops(rw);
#endif  // DEBUG_PHYSFS

    return 0;
} /* physfsrwops_close */

#ifdef USE_SDL2
static PHYSFSINT physfsrwops_size(SDL_RWops *const rw)
{
    PHYSFS_file *const handle = static_cast<PHYSFS_file*>(
        rw->hidden.unknown.data1);
    return VirtFs::fileLength(handle);
} /* physfsrwops_size */
#endif  // USE_SDL2

static SDL_RWops *create_rwops(PHYSFS_file *const handle)
{
    SDL_RWops *retval = nullptr;

    if (!handle)
    {
        logger->assertLog("physfsrwops_seek: create rwops error: %s",
            VirtFs::getLastError());
    }
    else
    {
        retval = SDL_AllocRW();
        if (retval)
        {
#ifdef USE_SDL2
            retval->size  = &physfsrwops_size;
#endif  // USE_SDL2

            retval->seek  = &physfsrwops_seek;
            retval->read  = &physfsrwops_read;
            retval->write = &physfsrwops_write;
            retval->close = &physfsrwops_close;
            retval->hidden.unknown.data1 = handle;
        } /* if */
#ifdef DUMP_LEAKED_RESOURCES
        openedRWops ++;
#endif  // DUMP_LEAKED_RESOURCES
    } /* else */

    return retval;
} /* create_rwops */

SDL_RWops *VirtFs::MakeRWops(PHYSFS_file *const handle)
{
    SDL_RWops *retval = nullptr;
    if (!handle)
    {
        logger->assertLog("physfsrwops_seek: NULL pointer passed to "
            "RWopsmakeRWops().");
    }
    else
    {
        retval = create_rwops(handle);
    }

    return retval;
} /* RWopsmakeRWops */

#ifdef __APPLE__
static bool checkFilePath(const char *const fname)
{
    if (!fname || !*fname)
        return false;
    if (!VirtFs::exists(fname) || VirtFs::isDirectory(fname))
        return false;
    return true;
}
#endif  // __APPLE__

#ifdef DEBUG_PHYSFS
#undef RWopsOpenRead
SDL_RWops *VirtFs::RWopsOpenRead(const char *const fname,
                                 const char *restrict const file,
                                 const unsigned line)
#else  // DEBUG_PHYSFS
SDL_RWops *VirtFs::RWopsOpenRead(const char *const fname)
#endif  // DEBUG_PHYSFS
{
    BLOCK_START("RWopsopenRead")
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__
#ifdef USE_FUZZER
    if (Fuzzer::conditionTerminate(fname))
        return nullptr;
#endif  // USE_FUZZER
#ifdef USE_PROFILER

#ifdef DEBUG_PHYSFS
    SDL_RWops *const ret = addDebugRWops(
        create_rwops(VirtFs::openRead(fname)),
        fname,
        file,
        line);
#else  // DEBUG_PHYSFS
    SDL_RWops *const ret = create_rwops(VirtFs::openRead(fname));
#endif  // DEBUG_PHYSFS

    BLOCK_END("RWopsopenRead")
    return ret;
#else  // USE_PROFILER

#ifdef DEBUG_PHYSFS
    return addDebugRWops(
        create_rwops(VirtFs::openRead(fname)),
        fname,
        file,
        line);
#else  // DEBUG_PHYSFS
    return create_rwops(VirtFs::openRead(fname));
#endif  // DEBUG_PHYSFS
#endif  // USE_PROFILER
} /* RWopsopenRead */

SDL_RWops *VirtFs::RWopsOpenWrite(const char *const fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openWrite(fname));
} /* RWopsopenWrite */

SDL_RWops *VirtFs::RWopsOpenAppend(const char *const fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openAppend(fname));
} /* RWopsopenAppend */

#ifdef DUMP_LEAKED_RESOURCES
void VirtFs::reportRWops()
{
    if (openedRWops)
    {
        logger->assertLog("physfsrwops_seek: leaking RWops: %d",
            openedRWops);
    }
}
#endif  // DUMP_LEAKED_RESOURCES

/* end of physfsrwops.c ... */
