/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef NET_EA_ADMINHANDLER_H
#define NET_EA_ADMINHANDLER_H

#include "net/adminhandler.h"
#include "net/net.h"

namespace Ea
{

class AdminHandler : public Net::AdminHandler
{
    public:
        AdminHandler()
        { }

        A_DELETE_COPY(AdminHandler)

        virtual ~AdminHandler()
        { }

        virtual void kickName(const std::string &name);

        virtual void ban(const int playerId);

        virtual void banName(const std::string &name);

        virtual void unban(const int playerId);

        virtual void unbanName(const std::string &name);

        virtual void mute(const int playerId, const int type,
                          const int limit);

        virtual void warp(const std::string &map,
                          const int x, const int y);
};

} // namespace Ea

#endif // NET_EA_ADMINHANDLER_H
