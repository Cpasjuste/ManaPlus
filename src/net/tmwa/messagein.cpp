/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "net/tmwa/messagein.h"

#include "net/packetcounters.h"

#include "log.h"

#include "utils/stringutils.h"

#include <SDL.h>
#include <SDL_endian.h>

#include "debug.h"

#define MAKEWORD(low, high) \
    ((unsigned short)(((unsigned char)(low)) | \
    ((unsigned short)((unsigned char)(high))) << 8))

namespace TmwAthena 
{

MessageIn::MessageIn(const char *data, unsigned int length):
    Net::MessageIn(data, length)
{
    // Read the message ID
    mId = readInt16();
}

Sint16 MessageIn::readInt16()
{
    Sint16 value = -1;
    if (mPos + 2 <= mLength)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Sint16 swap;
        memcpy(&swap, mData + mPos, sizeof(Sint16));
        value = SDL_Swap16(swap);
#else
        memcpy(&value, mData + mPos, sizeof(Sint16));
#endif
    }
    mPos += 2;
    PacketCounters::incInBytes(2);
    DEBUGLOG("readInt16: " + toString(static_cast<int>(value)));
    return value;
}

int MessageIn::readInt32()
{
    Sint32 value = -1;
    if (mPos + 4 <= mLength)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Sint32 swap;
        memcpy(&swap, mData + mPos, sizeof(Sint32));
        value = SDL_Swap32(swap);
#else
        memcpy(&value, mData + mPos, sizeof(Sint32));
#endif
    }
    mPos += 4;
    PacketCounters::incInBytes(4);
    DEBUGLOG(strprintf("readInt32: %u", value));
    return value;
}

} // namespace TmwAthena
