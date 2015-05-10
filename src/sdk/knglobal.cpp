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
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>

#include "kncodestylemanager.h"
#include "knlanguagemanager.h"
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knconfiguremanager.h"

#include "knglobal.h"

KNGlobal *KNGlobal::m_instance=nullptr;

KNGlobal *KNGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNGlobal:m_instance;
}

QString KNGlobal::simplifiedPath(const QString &path)
{
    return QFileInfo(path).absoluteFilePath();
}

QPalette KNGlobal::getPalette(const QString &caption)
{
    return m_themeManager->getPalette(caption);
}

QTextCharFormat KNGlobal::getFormat(const QString &name)
{
    return m_codeStyleManager->getFormat(name);
}

KNLanguageMode *KNGlobal::getLanguageMode(const QString &suffix)
{
    return m_languageManager->getLanguageMode(suffix);
}

QString KNGlobal::ensurePathAvaliable(const QString &path)
{
    //Check if there's a file named the same as the path.
    QFileInfo detectInfo(path);
    if(detectInfo.isFile())
    {
        //Remove the file first.
        if(!QFile(detectInfo.absoluteFilePath()).remove())
        {
            return QString();
        }
    }
    //Check if the directory has already exist.
    if(detectInfo.isDir() && detectInfo.exists())
    {
        return detectInfo.absoluteFilePath();
    }
    //Generate the folder.
    QDir detectFolder(detectInfo.absoluteFilePath());
    return detectFolder.mkpath(detectFolder.absolutePath())?
                detectFolder.absolutePath():QString();
}

void KNGlobal::loadTheme(const QString &themeFilePath)
{
    //Load the theme.
    m_themeManager->loadTheme(themeFilePath);
    //Apply the application sets.
    QApplication::setPalette(getPalette("Application"));
}

void KNGlobal::loadCodeStyle(const QString &codeStylePath)
{
    m_codeStyleManager->loadCodeStyle(codeStylePath);
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent),
    m_configureManager(KNConfigureManager::instance()),
    m_themeManager(KNThemeManager::instance()),
    m_localeManager(KNLocaleManager::instance()),
    m_languageManager(KNLanguageManager::instance()),
    m_codeStyleManager(KNCodeStyleManager::instance())
{
    //Configure the running environment.
    initialDefaultPath();

    //Load the configure.
    m_configureManager->setConfigureDir(ensurePathAvaliable(m_configureDir));

    //Load the default theme.
    loadTheme(":/configure/resource/theme/default.json");
    loadCodeStyle(":/configure/resource/code_style/default_style.json");

    //Load language and link the language update request.
    connect(m_localeManager, &KNLocaleManager::languageUpdate,
            this, &KNGlobal::languageUpdate);
}

inline void KNGlobal::initialDefaultPath()
{
#ifdef Q_OS_MACX
    m_userDataDir=
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
            + "/.kreogist/Cuties";
#endif
    m_configureDir=simplifiedPath(m_userDataDir+"/Configure");
    m_resourceDir=simplifiedPath(m_userDataDir+"/Resources");
}
