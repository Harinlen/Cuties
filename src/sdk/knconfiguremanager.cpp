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
#include "knconfigure.h"

#include "knconfiguremanager.h"

KNConfigureManager *KNConfigureManager::m_instance=nullptr;

KNConfigureManager *KNConfigureManager::instance()
{
    return m_instance==nullptr?m_instance=new KNConfigureManager:m_instance;
}

void KNConfigureManager::setConfigureDir(const QString &dirPath)
{
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configure[i]->setFilePath(dirPath + "/" + m_configureFileName[i]);
    }
}

void KNConfigureManager::loadConfigure()
{
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configure[i]->loadConfigure();
    }
}

void KNConfigureManager::saveConfigure()
{
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configure[i]->saveConfigure();
    }
}

KNConfigureManager::KNConfigureManager(QObject *parent) :
    QObject(parent)
{
    //Initial the file names.
    m_configureFileName[Cache]="cache.json";
    m_configureFileName[System]="system.json";
    m_configureFileName[User]="user.json";

    //Initial all the configures.
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configure[i]=new KNConfigure(this);
    }
}
