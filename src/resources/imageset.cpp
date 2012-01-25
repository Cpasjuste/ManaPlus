/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "resources/imageset.h"

#include "logger.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include "debug.h"

ImageSet::ImageSet(Image *img, int width, int height,
                   int margin, int spacing) :
    mOffsetX(0),
    mOffsetY(0)
{
    if (img)
    {
        for (int y = margin; y + height <= img->getHeight() - margin;
             y += height + spacing)
        {
            for (int x = margin; x + width <= img->getWidth() - margin;
                 x += width + spacing)
            {
                mImages.push_back(img->getSubImage(x, y, width, height));
            }
        }
    }
    mWidth = width;
    mHeight = height;
}

ImageSet::~ImageSet()
{
    delete_all(mImages);
}

Image* ImageSet::get(size_type i) const
{
    if (i >= mImages.size())
    {
        logger->log("Warning: No sprite %d in this image set",
            static_cast<int>(i));
        return nullptr;
    }
    else
    {
        return mImages[i];
    }
}
