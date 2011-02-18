/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef NET_MANASERV_GUILDHANDLER_H
#define NET_MANASERV_GUILDHANDLER_H

#include "net/guildhandler.h"

#include "net/manaserv/messagehandler.h"

#ifdef __GNUC__
#define _UNUSED_  __attribute__ ((unused))
#else
#define _UNUSED_
#endif

namespace ManaServ
{

class GuildHandler : public Net::GuildHandler, public MessageHandler
{
public:
    GuildHandler();

    bool isSupported()
    { return true; }

    void handleMessage(Net::MessageIn &msg);

    void create(const std::string &name);

    void invite(int guildId, const std::string &name);

    void invite(int guidId, Being *being);

    void inviteResponse(int guidId, bool response);

    void leave(int guildId);

    void kick(GuildMember *member, std::string reason = "");

    void chat(int guildId, const std::string &text);

    void memberList(int guildId);

    void info(int guildId _UNUSED_);

    void changeMemberPostion(GuildMember *member, int level);

    void requestAlliance(int guildId, int otherGuildId);

    void requestAllianceResponse(int guildId, int otherGuildId,
                                 bool response);

    void endAlliance(int guildId, int otherGuildId);

    void changeNotice(int guildId, std::string msg1,
                      std::string msg2);

protected:
    void joinedGuild(Net::MessageIn &msg);
};

} // namespace ManaServ

#endif
