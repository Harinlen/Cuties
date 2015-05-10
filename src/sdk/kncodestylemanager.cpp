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
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "knutilities.h"

#include "kncodestylemanager.h"

KNCodeStyleManager *KNCodeStyleManager::m_instance=nullptr;

KNCodeStyleManager *KNCodeStyleManager::instance()
{
    return m_instance==nullptr?m_instance=new KNCodeStyleManager:m_instance;
}

QTextCharFormat KNCodeStyleManager::getFormat(const QString &name)
{
    return m_codeStyle.value(name);
}

void KNCodeStyleManager::loadCodeStyle(const QString &styleFilePath)
{
    //Check the style file exist or not.
    QFile styleFile(styleFilePath);
    //Open the file as ReadOnly mode.
    if(!styleFile.exists() || !styleFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Read the data and close the file.
    QJsonObject styleData=QJsonDocument::fromJson(styleFile.readAll()).object();
    styleFile.close();
    //Load all the code style.
    QMap<QString, QTextCharFormat> codeStyle;
    //Parse the theme data.
    QStringList rulesNames=styleData.keys();
    for(QStringList::iterator i=rulesNames.begin();
        i!=rulesNames.end();
        ++i)
    {
        if(codeStyle.contains(*i))
        {
            continue;
        }
        //Parse the style.
        parseStyle(*i, styleData.value(*i).toString(), codeStyle);
    }
    m_codeStyle=codeStyle;
}

void KNCodeStyleManager::parseStyle(const QString &ruleName,
                                    const QString &ruleData,
                                    QMap<QString, QTextCharFormat> map)
{
    QTextCharFormat textFormat;
    textFormat.setForeground(KNUtilities::parseColor(ruleData));
    map.insert(ruleName, textFormat);
}

KNCodeStyleManager::KNCodeStyleManager(QObject *parent) :
    QObject(parent)
{
}
