/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "imageparticle.h"

#include "graphics.h"

#include "resources/image.h"

#include "debug.h"

std::map<std::string, int> ImageParticle::imageParticleCountByName;

ImageParticle::ImageParticle(Map *map, Image *image):
    Particle(map),
    mImage(image)
{
    if (mImage)
    {
        mImage->incRef();

        std::string name = mImage->getIdPath();
        if (ImageParticle::imageParticleCountByName.find(name)
            == ImageParticle::imageParticleCountByName.end())
        {
            ImageParticle::imageParticleCountByName[name] = 1;
        }
        else
        {
            ImageParticle::imageParticleCountByName[name] ++;
        }
    }
}

ImageParticle::~ImageParticle()
{
    if (mImage)
    {
        std::string name = mImage->getIdPath();
        if (ImageParticle::imageParticleCountByName[name] > 0)
            ImageParticle::imageParticleCountByName[name] --;

        mImage->decRef();
        mImage = nullptr;
    }
    setMap(nullptr);
}

bool ImageParticle::draw(Graphics *graphics, int offsetX, int offsetY) const
{
    if (!isAlive() || !mImage)
        return false;

    int screenX = static_cast<int>(mPos.x) + offsetX - mImage->mBounds.w / 2;
    int screenY = static_cast<int>(mPos.y) - static_cast<int>(mPos.z)
        + offsetY - mImage->mBounds.h / 2;

    // Check if on screen
    if (screenX + mImage->mBounds.w < 0 ||
        screenX > graphics->mWidth ||
        screenY + mImage->mBounds.h < 0 ||
        screenY > graphics->mHeight)
    {
        return false;
    }

    float alphafactor = mAlpha;

    if (mFadeOut && mLifetimeLeft > -1 && mLifetimeLeft < mFadeOut)
    {
        alphafactor *= static_cast<float>(mLifetimeLeft)
            / static_cast<float>(mFadeOut);
    }

    if (mFadeIn && mLifetimePast < mFadeIn)
    {
        alphafactor *= static_cast<float>(mLifetimePast)
        / static_cast<float>(mFadeIn);
    }

    mImage->setAlpha(alphafactor);
    return graphics->drawImage(mImage, screenX, screenY);
}
