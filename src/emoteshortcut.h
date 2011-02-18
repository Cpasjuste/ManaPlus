/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef EMOTESHORTCUT_H
#define EMOTESHORTCUT_H

#define SHORTCUT_EMOTES 44

/**
 * The class which keeps track of the emote shortcuts.
 */
class EmoteShortcut
{
    public:
        /**
         * Constructor.
         */
        EmoteShortcut();

        /**
         * Destructor.
         */
        ~EmoteShortcut();

        /**
         * Load the configuration information.
         */
        void load();

        /**
         * Returns the shortcut Emote ID specified by the index.
         *
         * @param index Index of the shortcut Emote.
         */
        unsigned char getEmote(int index) const
        { return mEmotes[index]; }

        /**
         * Returns the amount of shortcut Emotes.
         */
        unsigned int getEmoteCount() const
        { return SHORTCUT_EMOTES; }

        /**
         * Returns the emote ID that is currently selected.
         */
        unsigned char getEmoteSelected() const
        { return mEmoteSelected; }

        /**
         * Adds the selected emote ID to the emotes specified by the index.
         *
         * @param index Index of the emotes.
         */
        void setEmote(int index)
        { mEmotes[index] = mEmoteSelected; }

        /**
         * Adds a emoticon to the emotes store specified by the index.
         *
         * @param index Index of the emote.
         * @param emoteId ID of the emote.
         */
        void setEmotes(int index, unsigned char emoteId)
        { mEmotes[index] = emoteId; }

        /**
         * Set the Emote that is selected.
         *
         * @param emoteId The ID of the emote that is to be assigned.
         */
        void setEmoteSelected(unsigned char emoteId)
        { mEmoteSelected = emoteId; }

        /**
         * A flag to check if the Emote is selected.
         */
        bool isEmoteSelected() const
        { return mEmoteSelected; }

        /**
         * Remove a Emote from the shortcut.
         */
        void removeEmote(int index)
        { if (index >= 0 && index < SHORTCUT_EMOTES) mEmotes[index] = 0; }

        /**
         * Try to use the Emote specified by the index.
         *
         * @param index Index of the emote shortcut.
         */
        void useEmote(int index);

    private:
        /**
         * Save the configuration information.
         */
        void save();

        unsigned char mEmotes[SHORTCUT_EMOTES];  /**< The emote stored. */
        unsigned char mEmoteSelected;            /**< The emote held
                                                      by cursor. */

};

extern EmoteShortcut *emoteShortcut;

#endif
