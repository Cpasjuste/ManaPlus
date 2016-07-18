/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "resources/skill/skillinfo.h"

#include "being/playerinfo.h"

#include "gui/models/skillmodel.h"

#include "utils/stringutils.h"

#include "resources/skill/skilldata.h"
#include "resources/skill/skilltypelist.h"

#include "debug.h"

SkillInfo::SkillInfo() :
    skillLevel(),
    skillEffect(),
    useButton(),
    errorText(),
    castingAction(),
    castingRideAction(),
    castingSkyAction(),
    castingWaterAction(),
    dataMap(),
    model(nullptr),
    data(nullptr),
    level(0),
    selectedLevel(0),
    skillLevelWidth(0),
    id(0),
    range(0),
    sp(0),
    duration(0),
    durationTime(0),
    cooldown(0),
    castingSrcEffectId(-1),
    castingDstEffectId(-1),
    type(SkillType::Unknown),
    owner(SkillOwner::Player),
    modifiable(Modifiable_false),
    visible(Visible_false),
    alwaysVisible(Visible_false),
    useTextParameter(false)
{
    dataMap[0] = new SkillData();
    data = dataMap[0];
}

SkillInfo::~SkillInfo()
{
    FOR_EACH (SkillDataMapIter, it, dataMap)
        delete (*it).second;
    dataMap.clear();
}

void SkillInfo::update()
{
    const int baseLevel = PlayerInfo::getSkillLevel(id);
    if (modifiable == Modifiable_false && baseLevel == 0)
    {
        if (visible == Visible_true)
        {
            visible = Visible_false;
            if (model)
                model->updateVisibilities();
        }
        return;
    }

    const bool updateVisibility = (visible == Visible_false);
    visible = Visible_true;

    if (baseLevel == 0)
    {
        skillLevel.clear();
    }
    else
    {
        if (!selectedLevel)
        {
            // TRANSLATORS: skill level
            skillLevel = strprintf(_("Lvl: %d"), baseLevel);
        }
        else
        {
            // TRANSLATORS: skill level
            skillLevel = strprintf(_("Lvl: %d / %d"),
                selectedLevel,
                baseLevel);
        }
    }

    // TRANSLATORS: skill type
    const char *const typeStr = _("Type: %s");

    if (type == SkillType::Unknown)
    {
        // TRANSLATORS: Skill type
        skillEffect = strprintf(typeStr, _("Unknown"));
    }
    else
    {
        for (size_t f = 0; f < skillTypeListSize; f ++)
        {
            const SkillTypeEntry &item = skillTypeList[f];
            if ((item.type & type) != 0)
            {
                if (!skillEffect.empty())
                    skillEffect.append(", ");
                skillEffect.append(strprintf(typeStr, item.name));
            }
        }
    }
    if (skillEffect.empty())
    {
        // TRANSLATORS: Skill type
        skillEffect = strprintf(typeStr, _("Unknown:"));
        skillEffect.append(" ").append(toString(CAST_S32(type)));
    }

    if (sp)
    {
        // TRANSLATORS: skill mana
        skillEffect.append(strprintf(_(" / Mana: -%d"), sp));
    }

    if (range > 0)
    {
        if (!skillEffect.empty())
            skillEffect.append(" / ");
        // TRANSLATORS: skill range
        skillEffect.append(strprintf(_("Range: %d"), range));
    }

    level = baseLevel;
    if (selectedLevel > level)
        selectedLevel = level;

    skillLevelWidth = -1;

    if (updateVisibility && model)
        model->updateVisibilities();

    data = getData(level);
    if (!data)
        data = dataMap[0];
}


void SkillInfo::addData(const int level1, SkillData *const data1)
{
    dataMap[level1] = data1;
}

SkillData *SkillInfo::getData(const int level1) const
{
    const SkillDataMapCIter it = dataMap.find(level1);
    if (it == dataMap.end())
        return nullptr;
    return (*it).second;
}

SkillData *SkillInfo::getData1(const int lev) const
{
    const SkillDataMapCIter it = dataMap.find(lev);
    if (it == dataMap.end())
        return (*dataMap.begin()).second;
    return (*it).second;
}