/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

//Highlighters
#include "kncpphighlighter.h"

#include "knlanguagemode.h"

#include "knlanguagemanager.h"

KNLanguageManager *KNLanguageManager::m_instance=nullptr;

KNLanguageManager *KNLanguageManager::instance()
{
    return m_instance==nullptr?m_instance=new KNLanguageManager:m_instance;
}

KNLanguageMode *KNLanguageManager::getLanguageMode(QString suffix)
{
    //Get the lower case of the suffix.
    suffix=suffix.toLower();
    KNLanguageMode *mode=new KNLanguageMode;
    if(suffix=="cpp" || suffix=="h")
    {
        mode->setHighlighter(new KNCppHighlighter(mode));
        if(suffix=="cpp")
        {
            mode->setLanguageName("C++");
        }
        else if(suffix=="h")
        {
            mode->setLanguageName("C/C++ Header");
        }
    }
    else
    {
        mode->setHighlighter(new KNHighlighter(mode));
    }
    return mode;
}

KNLanguageManager::KNLanguageManager(QObject *parent) :
    QObject(parent)
{
    ;
}
