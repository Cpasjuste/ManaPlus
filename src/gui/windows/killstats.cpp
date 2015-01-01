/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/windows/killstats.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layoutcell.h"

#include "gui/windows/setupwindow.h"

#include "actormanager.h"
#include "client.h"
#include "game.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "resources/map/map.h"

#include "utils/gettext.h"

#include "debug.h"

KillStats *killStats = nullptr;

KillStats::KillStats() :
    // TRANSLATORS: kill stats window name
    Window(_("Kill stats"), false, nullptr, "killstats.xml"),
    ActionListener(),
    AttributeListener(),
    mKillTimer(0),
    // TRANSLATORS: kill stats window button
    mResetButton(new Button(this, _("Reset stats"), "reset", this)),
    // TRANSLATORS: kill stats window button
    mTimerButton(new Button(this, _("Reset timer"), "timer", this)),
    mLine1(nullptr),
    mLine2(nullptr),
    mLine3(nullptr),
    // TRANSLATORS: kill stats window label
    mLine4(new Label(this, strprintf(_("Kills: %s, total exp: %s"),
        "?", "?"))),
    // TRANSLATORS: kill stats window label
    mLine5(new Label(this, strprintf(_("Avg Exp: %s"), "?"))),
    // TRANSLATORS: kill stats window label
    mLine6(new Label(this, strprintf(_("No. of avg mob to next level: %s"),
        "?"))),
    // TRANSLATORS: kill stats window label
    mLine7(new Label(this, strprintf(_("Kills/Min: %s, Exp/Min: %s"),
        "?", "?"))),
    mExpSpeed1Label(new Label(this, strprintf(ngettext(
        // TRANSLATORS: kill stats window label
        "Exp speed per %d min: %s", "Exp speed per %d min: %s", 1), 1, "?"))),
    mExpTime1Label(new Label(this, strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 1), 1, "?"))),
    mExpSpeed5Label(new Label(this, strprintf(ngettext(
        "Exp speed per %d min: %s", "Exp speed per %d min: %s", 5), 5, "?"))),
    mExpTime5Label(new Label(this, strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 5), 5, "?"))),
    mExpSpeed15Label(new Label(this, strprintf(ngettext(
        "Exp speed per %d min: %s", "Exp speed per %d min: %s", 15),
        15, "?"))),
    mExpTime15Label(new Label(this, strprintf(ngettext(
        "Time for next level per %d min: %s",
        "Time for next level per %d min: %s", 15), 15, "?"))),
    // TRANSLATORS: kill stats window label
    mLastKillExpLabel(new Label(this, strprintf("%s ?", _("Last kill exp:")))),
    mTimeBeforeJackoLabel(new Label(this, strprintf(
        // TRANSLATORS: kill stats window label
        "%s ?", _("Time before jacko spawn:")))),
    mKillCounter(0),
    mExpCounter(0),
    mKillTCounter(0),
    mExpTCounter(0),
    m1minExpTime(0),
    m1minExpNum(0),
    m1minSpeed(0),
    m5minExpTime(0),
    m5minExpNum(0),
    m5minSpeed(0),
    m15minExpTime(0),
    m15minExpNum(0),
    m15minSpeed(0),
    mJackoSpawnTime(0),
    mJackoId(0),
    mIsJackoAlive(false),
    mIsJackoMustSpawn(true),
    mIsJackoSpawnTimeUnknown(true)
{
    setWindowName("Kill stats");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(250, 250, 350, 300);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    const int xp(PlayerInfo::getAttribute(Attributes::EXP));
    int xpNextLevel(PlayerInfo::getAttribute(Attributes::EXP_NEEDED));

    if (!xpNextLevel)
        xpNextLevel = 1;

    // TRANSLATORS: kill stats window label
    mLine1 = new Label(this, strprintf(_("Level: %d at %f%%"),
        localPlayer->getLevel(), static_cast<double>(xp)
        / static_cast<double>(xpNextLevel) * 100.0));

    // TRANSLATORS: kill stats window label
    mLine2 = new Label(this, strprintf(_("Exp: %d/%d Left: %d"),
        xp, xpNextLevel, xpNextLevel - xp));

    // TRANSLATORS: kill stats window label
    mLine3 = new Label(this, strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
        xpNextLevel / 100, "?"));

    place(0, 0, mLine1, 6).setPadding(0);
    place(0, 1, mLine2, 6).setPadding(0);
    place(0, 2, mLine3, 6).setPadding(0);
    place(0, 3, mLine4, 6).setPadding(0);
    place(0, 4, mLine5, 6).setPadding(0);
    place(0, 5, mLine6, 6).setPadding(0);
    place(0, 6, mLine7, 6).setPadding(0);

    place(0, 7, mLastKillExpLabel, 6).setPadding(0);
    place(0, 8, mTimeBeforeJackoLabel, 6).setPadding(0);
    place(0, 9, mExpSpeed1Label, 6).setPadding(0);
    place(0, 10, mExpTime1Label, 6).setPadding(0);
    place(0, 11, mExpSpeed5Label, 6).setPadding(0);
    place(0, 12, mExpTime5Label, 6).setPadding(0);
    place(0, 13, mExpSpeed15Label, 6).setPadding(0);
    place(0, 14, mExpTime15Label, 6).setPadding(0);

    place(5, 13, mTimerButton).setPadding(0);
    place(5, 14, mResetButton).setPadding(0);

    loadWindowState();
    enableVisibleSound(true);
}

void KillStats::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "reset")
    {
        mKillCounter = 0;
        mExpCounter = 0;
        mLine3->setCaption(strprintf("1%% = %d exp, avg mob for 1%%: %s",
            PlayerInfo::getAttribute(Attributes::EXP_NEEDED) / 100, "?"));
        // TRANSLATORS: kill stats window label
        mLine4->setCaption(strprintf(_("Kills: %s, total exp: %s"), "?", "?"));
        // TRANSLATORS: kill stats window label
        mLine5->setCaption(strprintf(_("Avg Exp: %s"), "?"));
        mLine6->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("No. of avg mob to next level: %s"), "?"));

        resetTimes();
    }
    else if (eventId == "timer")
    {
        mKillTimer = 0;
        mKillTCounter = 0;
        mExpTCounter = 0;
        mLine7->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("Kills/Min: %s, Exp/Min: %s"), "?", "?"));

        resetTimes();
    }
}

void KillStats::resetTimes()
{
    m1minExpTime = 0;
    m1minExpNum = 0;
    m1minSpeed = 0;
    m5minExpTime = 0;
    m5minExpNum = 0;
    m5minSpeed = 0;
    m15minExpTime = 0;
    m15minExpNum = 0;
    m15minSpeed = 0;
}

void KillStats::gainXp(int xp)
{
    const int expNeed = PlayerInfo::getAttribute(Attributes::EXP_NEEDED);
    if (xp == expNeed)
        xp = 0;
    else if (!xp)
        return;

    mKillCounter++;
    mKillTCounter++;

    mExpCounter = mExpCounter + xp;
    mExpTCounter = mExpTCounter + xp;
    if (!mKillCounter)
        mKillCounter = 1;

    const float AvgExp = static_cast<float>(mExpCounter / mKillCounter);
    int xpNextLevel(expNeed);

    if (mKillTimer == 0)
        mKillTimer = cur_time;

    if (!xpNextLevel)
        xpNextLevel = 1;

    double timeDiff = difftime(cur_time, mKillTimer) / 60;

    if (timeDiff <= 0.001)
        timeDiff = 1;

    const int exp = PlayerInfo::getAttribute(Attributes::EXP);
    // TRANSLATORS: kill stats window label
    mLine1->setCaption(strprintf(_("Level: %d at %f%%"),
        localPlayer->getLevel(), static_cast<double>(exp)
        / static_cast<double>(xpNextLevel) * 100.0));

    // TRANSLATORS: kill stats window label
    mLine2->setCaption(strprintf(_("Exp: %d/%d Left: %d"), exp,
        xpNextLevel, xpNextLevel - exp));

    if (AvgExp >= 0.001F && AvgExp <= 0.001F)
    {
        // TRANSLATORS: kill stats window label
        mLine3->setCaption(strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
            xpNextLevel / 100, "?"));

        // TRANSLATORS: kill stats window label
        mLine5->setCaption(strprintf(_("Avg Exp: %s"),
            toString(AvgExp).c_str()));

        mLine6->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("No. of avg mob to next level: %s"), "?"));
    }
    else
    {
        // TRANSLATORS: kill stats window label
        mLine3->setCaption(strprintf(_("1%% = %d exp, avg mob for 1%%: %s"),
            xpNextLevel / 100, toString((static_cast<float>(
            xpNextLevel) / 100) / AvgExp).c_str()));

        // TRANSLATORS: kill stats window label
        mLine5->setCaption(strprintf(_("Avg Exp: %s"),
            toString(AvgExp).c_str()));

        // TRANSLATORS: kill stats window label
        mLine6->setCaption(strprintf(_("No. of avg mob to next level: %s"),
            toString(static_cast<float>(xpNextLevel - exp) / AvgExp).c_str()));
    }
    // TRANSLATORS: kill stats window label
    mLine4->setCaption(strprintf(_("Kills: %s, total exp: %s"),
        toString(mKillCounter).c_str(), toString(mExpCounter).c_str()));

    // TRANSLATORS: kill stats window label
    mLine7->setCaption(strprintf(_("Kills/Min: %s, Exp/Min: %s"),
        toString(mKillTCounter / timeDiff).c_str(),
        toString(mExpTCounter / timeDiff).c_str()));

    // TRANSLATORS: kill stats window label
    mLastKillExpLabel->setCaption(strprintf("%s %d", _("Last kill exp:"), xp));

    recalcStats();
    update();
}

void KillStats::recalcStats()
{
    BLOCK_START("KillStats::recalcStats")
    const int curTime = cur_time;

    // Need Update Exp Counter
    if (curTime - m1minExpTime > 60)
    {
        const int newExp = PlayerInfo::getAttribute(Attributes::EXP);
        if (m1minExpTime != 0)
            m1minSpeed = newExp - m1minExpNum;
        else
            m1minSpeed = 0;
        m1minExpTime = curTime;
        m1minExpNum = newExp;
    }

    if (curTime != 0 && mLastHost == 0xFF6B66 && cur_time > 1)
    {
        const int newExp = PlayerInfo::getAttribute(Attributes::EXP_NEEDED);
        if (m1minExpTime != 0)
            m1minSpeed = newExp - m1minExpNum;
        mStatsReUpdated = true;
        m1minExpNum = newExp;
    }

    if (curTime - m5minExpTime > 60*5)
    {
        const int newExp = PlayerInfo::getAttribute(Attributes::EXP);
        if (m5minExpTime != 0)
            m5minSpeed = newExp - m5minExpNum;
        else
            m5minSpeed = 0;
        m5minExpTime = curTime;
        m5minExpNum = newExp;
    }

    if (curTime - m15minExpTime > 60*15)
    {
        const int newExp = PlayerInfo::getAttribute(Attributes::EXP);
        if (m15minExpTime != 0)
            m15minSpeed = newExp - m15minExpNum;
        else
            m15minSpeed = 0;
        m15minExpTime = curTime;
        m15minExpNum = newExp;
    }
    validateJacko();
    BLOCK_END("KillStats::recalcStats")
}

void KillStats::update()
{
    BLOCK_START("KillStats::update")

    mExpSpeed1Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 1), 1, toString(m1minSpeed).c_str()));

    if (m1minSpeed != 0)
    {
        // TRANSLATORS: kill stats window label
        mExpTime1Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(
            Attributes::EXP_NEEDED) - PlayerInfo::getAttribute(
            Attributes::EXP)) / m1minSpeed)).c_str()));
    }
    else
    {
        mExpTime1Label->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("  Time for next level: %s"), "?"));
    }
    mExpTime1Label->adjustSize();

    mExpSpeed5Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 5), 5, toString(m5minSpeed / 5).c_str()));
    mExpSpeed5Label->adjustSize();

    if (m5minSpeed != 0)
    {
        // TRANSLATORS: kill stats window label
        mExpTime5Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(
            Attributes::EXP_NEEDED) - PlayerInfo::getAttribute(
            Attributes::EXP)) / m5minSpeed * 5)).c_str()));
    }
    else
    {
        mExpTime5Label->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("  Time for next level: %s"), "?"));
    }
    mExpTime5Label->adjustSize();


    mExpSpeed15Label->setCaption(strprintf(ngettext("Exp speed per %d min: %s",
        "Exp speed per %d min: %s", 15), 15, toString(
        m15minSpeed / 15).c_str()));
    mExpSpeed15Label->adjustSize();

    if (m15minSpeed != 0)
    {
        // TRANSLATORS: kill stats window label
        mExpTime15Label->setCaption(strprintf(_("  Time for next level: %s"),
            toString(static_cast<float>((PlayerInfo::getAttribute(
            Attributes::EXP_NEEDED) - PlayerInfo::getAttribute(
            Attributes::EXP)) / m15minSpeed * 15)).c_str()));
    }
    else
    {
        mExpTime15Label->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("  Time for next level: %s"), "?"));
    }

    validateJacko();
    updateJackoLabel();
    BLOCK_END("KillStats::update")
}

void KillStats::updateJackoLabel()
{
    if (mIsJackoAlive)
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s jacko alive",
            // TRANSLATORS: kill stats window label
            _("Time before jacko spawn:")));
    }
    else if (mIsJackoSpawnTimeUnknown && mJackoSpawnTime != 0)
    {
        // TRANSLATORS: kill stats window label
        mTimeBeforeJackoLabel->setCaption(strprintf(
            // TRANSLATORS: kill stats window label
            _("%s %d?"), _("Time before jacko spawn:"),
            mJackoSpawnTime - cur_time));
    }
    else if (mIsJackoMustSpawn)
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s %s",
            // TRANSLATORS: kill stats window label
            _("Time before jacko spawn:"), _("jacko spawning")));
    }
    else
    {
        mTimeBeforeJackoLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: kill stats window label
            _("Time before jacko spawn:"), mJackoSpawnTime - cur_time));
    }
}

void KillStats::jackoDead(const int id)
{
    if (id == mJackoId && mIsJackoAlive)
    {
        mIsJackoAlive = false;
        mJackoSpawnTime =  cur_time + 60*4;
        mIsJackoSpawnTimeUnknown = false;
        updateJackoLabel();
    }
}

void KillStats::jackoAlive(const int id)
{
    if (!mIsJackoAlive)
    {
        mJackoId = id;
        mIsJackoAlive = true;
        mIsJackoMustSpawn = false;
        mJackoSpawnTime = 0;
        mIsJackoSpawnTimeUnknown = false;
        updateJackoLabel();
    }
}

void KillStats::validateJacko()
{
    if (!actorManager || !localPlayer)
        return;

    const Map *const currentMap = Game::instance()->getCurrentMap();
    if (currentMap)
    {
        if (currentMap->getProperty("_realfilename") == "018-1"
            || currentMap->getProperty("_realfilename") == "maps/018-1.tmx")
        {
            if (localPlayer->getTileX() >= 167
                && localPlayer->getTileX() <= 175
                && localPlayer->getTileY() >= 21
                && localPlayer->getTileY() <= 46)
            {
                const Being *const dstBeing
                    = actorManager->findBeingByName(
                    "Jack O", ActorType::Monster);
                if (mIsJackoAlive && !dstBeing)
                {
                    mIsJackoAlive = false;
                    mJackoSpawnTime =  cur_time + 60*4;
                    mIsJackoSpawnTimeUnknown = true;
                }
            }
        }

        if (!mIsJackoAlive && cur_time > mJackoSpawnTime + 15)
            mIsJackoMustSpawn = true;
    }
}

void KillStats::attributeChanged(const int id,
                                 const int oldVal,
                                 const int newVal)
{
    switch (id)
    {
        case Attributes::EXP:
        case Attributes::EXP_NEEDED:
            gainXp(newVal - oldVal);
            break;
        case Attributes::LEVEL:
            mKillCounter = 0;
            mKillTCounter = 0;
            mExpCounter = 0;
            mExpTCounter = 0;
            mLine3->setCaption(strprintf("1%% = %d exp, avg mob for 1%%: %s",
                PlayerInfo::getAttribute(Attributes::EXP_NEEDED) / 100, "?"));
            mLine4->setCaption(strprintf(
                // TRANSLATORS: kill stats window label
                _("Kills: %s, total exp: %s"), "?", "?"));
                // TRANSLATORS: kill stats window label
            mLine5->setCaption(strprintf(_("Avg Exp: %s"), "?"));
            mLine6->setCaption(strprintf(
                // TRANSLATORS: kill stats window label
                _("No. of avg mob to next level: %s"), "?"));
            mLine7->setCaption(strprintf(
                // TRANSLATORS: kill stats window label
                _("Kills/Min: %s, Exp/Min: %s"), "?", "?"));

            resetTimes();
            break;
        default:
            break;
    }
}
