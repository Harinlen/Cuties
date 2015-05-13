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
#include <QStyleFactory>

//Dependences
#include "knversion.h"
#include "knglobal.h"
#include "knmainwindow.h"
#include "kntabmanager.h"
#include "kncodeeditor.h"

//Ports
#include "knwelcomebase.h"

//Plugins
#include "plugins/knwelcome/knwelcome.h"

#include "knpluginmanager.h"

KNPluginManager *KNPluginManager::m_instance=nullptr;

KNPluginManager *KNPluginManager::instance()
{
    return m_instance==nullptr?m_instance=new KNPluginManager:m_instance;
}

void KNPluginManager::loadPlugins()
{
    //Set tab manager.

    //Load welcome.
//    m_mainWindow->setWelcome(new KNWelcome);
}

KNMainWindow *KNPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KNPluginManager::setMainWindow(KNMainWindow *mainWindow)
{
    //Save the main window pointer.
    m_mainWindow=mainWindow;
    //Configure the main window.
    if(m_mainWindow==nullptr)
    {
        return;
    }
    m_mainWindow->setWindowTitle("Cuties");
    m_mainWindow->setPalette(m_global->getPalette(m_mainWindow->objectName()));
}

void KNPluginManager::start()
{
    //Check the main window is null.
    if(!m_mainWindow)
    {
        return;
    }
    //Show the main window.
    m_mainWindow->show();
}

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr),
    m_global(KNGlobal::instance())
{
    //Initial the application information.
    QApplication::setApplicationName("Cuties");
    QApplication::setApplicationVersion(APP_VERSION_STR);
    QApplication::setApplicationDisplayName("Cuties");
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");

    //Configure the global.
    QApplication::setStyle(QStyleFactory::create("fusion"));
}
