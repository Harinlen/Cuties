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
#include <QApplication>
#include <QFile>
#include <QLinkedList>
#include <QJsonDocument>
#include <QJsonObject>

#include "knthememanager.h"

KNThemeManager *KNThemeManager::m_instance=nullptr;

KNThemeManager *KNThemeManager::instance()
{
    return m_instance==nullptr?m_instance=new KNThemeManager:m_instance;
}

QPalette KNThemeManager::getPalette(const QString &name) const
{
    return m_palMap.value(name);
}

void KNThemeManager::loadTheme(const QString &themeFilePath)
{
    //Check the theme file exist or not.
    QFile themeFile(themeFilePath);
    //Open the file as ReadOnly mode.
    if(!themeFile.exists() || !themeFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Read the data and close the file.
    QJsonObject themeData=QJsonDocument::fromJson(themeFile.readAll()).object();
    themeFile.close();
    //Generate the temporary palette map and name stack.
    QHash<QString, QPalette> palMap;
    //Parse the theme data.
    QStringList paletteNames=themeData.keys();
    for(QStringList::iterator i=paletteNames.begin();
        i!=paletteNames.end();
        ++i)
    {
        //Check whether we have loaded this name before.
        if(palMap.contains(*i))
        {
            continue;
        }
        //Parse the data.
        parsePalette(*i, &themeData, palMap);
    }
    //Check the new map is available or not.
    if(palMap.isEmpty())
    {
        return;
    }
    //Save the palette map.
    m_palMap=palMap;
}

void KNThemeManager::parsePalette(const QString &name,
                                  QJsonObject *data,
                                  QHash<QString, QPalette> &map)
{
    //Get current object.
    QJsonObject palData=data->value(name).toObject();
    QPalette pal;
    //Check data contains parent or not. If it has, load the parent palette.
    if(palData.contains("Parent"))
    {
        QString parentName=palData.value("Parent").toString();
        if(!map.contains(parentName))
        {
            parsePalette(parentName, data, map);
        }
        pal=map.value(parentName);
    }
    else
    {
        pal=QApplication::palette();
    }
    //Load current palette.
    QStringList properties=palData.keys();
    for(QStringList::iterator i=properties.begin();
        i!=properties.end();
        ++i)
    {
        if(m_colorRoleMap.contains(*i))
        {
            pal.setColor(m_colorRoleMap.value(*i),
                         parseColor(palData.value(*i).toString()));
        }
    }
    //Insert the palette to the map.
    map.insert(name, pal);
}

QColor KNThemeManager::parseColor(const QString &data)
{
    switch(data.length())
    {
    case 3:
        //Format should be RGB.
        return QColor(data.mid(0,1).toInt(nullptr, 16)*16-1,
                      data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1);
    case 4:
        //Format should be #RGB.
        return QColor(data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1,
                      data.mid(3,1).toInt(nullptr, 16)*16-1);
    case 6:
        //Format should be RRGGBB.
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16));
    case 7:
        //Format should be #RRGGBB
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16));
    case 8:
        //Format should be RRGGBBAA
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16),
                      data.mid(6,2).toInt(nullptr, 16));
    case 9:
        //Format should be #RRGGBBAA
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16),
                      data.mid(7,2).toInt(nullptr, 16));
    default:
        //Or else return a default color;
        return QColor();
    }
}

KNThemeManager::KNThemeManager(QObject *parent) :
    QObject(parent)
{
    //Initial the color role map.
    m_colorRoleMap["WindowText"]=QPalette::WindowText;
    m_colorRoleMap["Button"]=QPalette::Button;
    m_colorRoleMap["Light"]=QPalette::Light;
    m_colorRoleMap["Midlight"]=QPalette::Midlight;
    m_colorRoleMap["Dark"]=QPalette::Dark;
    m_colorRoleMap["Mid"]=QPalette::Mid;
    m_colorRoleMap["Text"]=QPalette::Text;
    m_colorRoleMap["BrightText"]=QPalette::BrightText;
    m_colorRoleMap["ButtonText"]=QPalette::ButtonText;
    m_colorRoleMap["Base"]=QPalette::Base;
    m_colorRoleMap["Window"]=QPalette::Window;
    m_colorRoleMap["Shadow"]=QPalette::Shadow;
    m_colorRoleMap["Highlight"]=QPalette::Highlight;
    m_colorRoleMap["HighlightedText"]=QPalette::HighlightedText;
    m_colorRoleMap["Link"]=QPalette::Link;
    m_colorRoleMap["LinkVisited"]=QPalette::LinkVisited;
    m_colorRoleMap["AlternateBase"]=QPalette::AlternateBase;
    m_colorRoleMap["NoRole"]=QPalette::NoRole;
    m_colorRoleMap["ToolTipBase"]=QPalette::ToolTipBase;
    m_colorRoleMap["ToolTipText"]=QPalette::ToolTipText;
}
