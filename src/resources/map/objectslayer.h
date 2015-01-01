/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_OBJECTSLAYER_H
#define RESOURCES_MAP_OBJECTSLAYER_H

#include <string>

#include "localconsts.h"

class MapObjectList;

class ObjectsLayer final
{
    public:
        ObjectsLayer(const unsigned width, const unsigned height);

        A_DELETE_COPY(ObjectsLayer)

        ~ObjectsLayer();

        void addObject(const std::string &name, const int type,
                       const unsigned x, const unsigned y,
                       unsigned dx, unsigned dy);

        MapObjectList *getAt(const unsigned x,
                             const unsigned y) const A_WARN_UNUSED;
    private:
        MapObjectList **mTiles;
        unsigned mWidth;
        unsigned mHeight;
};

#endif  // RESOURCES_MAP_OBJECTSLAYER_H
