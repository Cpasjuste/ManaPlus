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

#ifndef NET_MANASERV_EFFECTSHANDLER_H
#define NET_MANASERV_EFFECTSHANDLER_H

#include "net/manaserv/messagehandler.h"

namespace ManaServ
{

class EffectHandler : public MessageHandler
{
    public:
        EffectHandler();

        void handleMessage(Net::MessageIn &msg);

    private:
        void handleCreateEffectPos(Net::MessageIn &msg);
        void handleCreateEffectBeing(Net::MessageIn &msg);
};

} // namespace ManaServ

#endif // NET_MANASERV_EFFECTSHANDLER_H
