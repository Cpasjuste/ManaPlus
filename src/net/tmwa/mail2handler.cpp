/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/mail2handler.h"

#include "debug.h"

namespace TmwAthena
{

Mail2Handler::Mail2Handler()
{
    mail2Handler = this;
}

Mail2Handler::~Mail2Handler()
{
    mail2Handler = nullptr;
}

void Mail2Handler::openWriteMail(const std::string &receiver A_UNUSED) const
{
}

void Mail2Handler::addItem(const Item *const item A_UNUSED,
                           const int amount A_UNUSED) const
{
}

void Mail2Handler::removeItem(const Item *const item A_UNUSED,
                              const int amount A_UNUSED) const
{
}

}  // namespace TmwAthena