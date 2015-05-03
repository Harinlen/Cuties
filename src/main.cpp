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

#include "knpluginmanager.h"
#include "knmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //Create an application pattern.
    QApplication app(argc, argv);
    //Create the main mainwindow.
    KNMainWindow mainWindow;
    //Initial the plugin manager, set the main window.
    KNPluginManager *pluginManager=KNPluginManager::instance();
    pluginManager->setMainWindow(&mainWindow);
    //Load plugins.
    pluginManager->loadPlugins();
    //Start application.
    pluginManager->start();
    //Spread message loop.
    int result=app.exec();
    //Remove the plugin manager and recover the memory.
    delete pluginManager;
    return result;
}
