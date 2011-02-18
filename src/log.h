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

#ifndef _LOG_H
#define _LOG_H

#include "main.h"
#include <fstream>

class ChatWindow;

#ifdef ENABLEDEBUGLOG
#define DEBUGLOG(msg) logger->dlog(msg)
#else
#define DEBUGLOG(msg) {}
#endif

/**
 * The Log Class : Useful to write debug or info messages
 */
class Logger
{
    public:
        /**
         * Constructor.
         */
        Logger();

        /**
         * Destructor, closes log file.
         */
        ~Logger();

        /**
         * Sets the file to log to and opens it
         */
        void setLogFile(const std::string &logFilename);

        /**
         * Sets whether the log should be written to standard output.
         */
        void setLogToStandardOut(bool value) { mLogToStandardOut = value; }

        /**
         * Enables logging to chat window
         */
        void setChatWindow(ChatWindow *window) { mChatWindow = window; }

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log(const char *log_text, ...)
#ifdef __GNUC__
            __attribute__((__format__(__printf__, 2, 3)))
#endif
            ;

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log1(const char *log_text);

        /**
         * Enters a message in the log. The message will be timestamped.
         */
        void log(std::string str);

        /**
         * Enters debug message in the log. The message will be timestamped.
         */
        void dlog(std::string str);

        void setDebugLog(bool n)
        { mDebugLog = n; }

        /**
         * Log an error and quit. The error will pop-up on Windows and Mac, and
         * will be printed to standard error everywhere else.
         */
        void error(const std::string &error_text) __attribute__ ((noreturn));

    private:
        std::ofstream mLogFile;
        bool mLogToStandardOut;
        ChatWindow *mChatWindow;
        bool mDebugLog;
};

extern Logger *logger;

#endif
