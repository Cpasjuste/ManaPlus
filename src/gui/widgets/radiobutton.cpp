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

#include "gui/widgets/radiobutton.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"

#include "gui/theme.h"

#include "resources/image.h"

int RadioButton::instances = 0;
float RadioButton::mAlpha = 1.0;
Image *RadioButton::radioNormal;
Image *RadioButton::radioChecked;
Image *RadioButton::radioDisabled;
Image *RadioButton::radioDisabledChecked;
Image *RadioButton::radioNormalHi;
Image *RadioButton::radioCheckedHi;

RadioButton::RadioButton(const std::string &caption, const std::string &group,
                         bool marked):
    gcn::RadioButton(caption, group, marked),
    mHasMouse(false)
{
    if (instances == 0)
    {
        radioNormal = Theme::getImageFromTheme("radioout.png");
        radioChecked = Theme::getImageFromTheme("radioin.png");
        radioDisabled = Theme::getImageFromTheme("radioout.png");
        radioDisabledChecked = Theme::getImageFromTheme("radioin.png");
        radioNormalHi = Theme::getImageFromTheme("radioout_highlight.png");
        radioCheckedHi = Theme::getImageFromTheme("radioin_highlight.png");
        if (radioNormal)
            radioNormal->setAlpha(mAlpha);
        if (radioChecked)
            radioChecked->setAlpha(mAlpha);
        if (radioDisabled)
            radioDisabled->setAlpha(mAlpha);
        if (radioDisabledChecked)
            radioDisabledChecked->setAlpha(mAlpha);
        if (radioNormalHi)
            radioNormalHi->setAlpha(mAlpha);
        if (radioCheckedHi)
            radioCheckedHi->setAlpha(mAlpha);
    }

    instances++;
}

RadioButton::~RadioButton()
{
    instances--;

    if (instances == 0)
    {
        if (radioNormal)
            radioNormal->decRef();
        if (radioChecked)
            radioChecked->decRef();
        if (radioDisabled)
            radioDisabled->decRef();
        if (radioDisabledChecked)
            radioDisabledChecked->decRef();
        if (radioNormalHi)
            radioNormalHi->decRef();
        if (radioCheckedHi)
            radioCheckedHi->decRef();
    }
}

void RadioButton::drawBox(gcn::Graphics* graphics)
{
    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        if (radioNormal)
            radioNormal->setAlpha(mAlpha);
        if (radioChecked)
            radioChecked->setAlpha(mAlpha);
        if (radioDisabled)
            radioDisabled->setAlpha(mAlpha);
        if (radioDisabledChecked)
            radioDisabledChecked->setAlpha(mAlpha);
        if (radioNormalHi)
            radioNormalHi->setAlpha(mAlpha);
        if (radioCheckedHi)
            radioCheckedHi->setAlpha(mAlpha);
    }

    Image *box = NULL;

    if (isEnabled())
    {
        if (isSelected())
            if (mHasMouse)
                box = radioCheckedHi;
            else
                box = radioChecked;
        else
            if (mHasMouse)
                box = radioNormalHi;
            else
                box = radioNormal;
    }
    else
    {
        if (isSelected())
            box = radioDisabledChecked;
        else
            box = radioDisabled;
    }

    if (box)
        static_cast<Graphics*>(graphics)->drawImage(box, 2, 2);
}

void RadioButton::draw(gcn::Graphics* graphics)
{
    graphics->pushClipArea(gcn::Rectangle(1, 1, getWidth() - 1,
                           getHeight() - 1));

    drawBox(graphics);

    graphics->popClipArea();

    graphics->setFont(getFont());
    graphics->setColor(getForegroundColor());

//    int h = getHeight() + getHeight() / 2;
//    graphics->drawText(getCaption(), h - 2, 0);
    graphics->drawText(getCaption(), 16, 0);
}

void RadioButton::mouseEntered(gcn::MouseEvent& event _UNUSED_)
{
    mHasMouse = true;
}

void RadioButton::mouseExited(gcn::MouseEvent& event _UNUSED_)
{
    mHasMouse = false;
}

