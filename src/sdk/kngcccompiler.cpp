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
#include <QDir>

#include "kngcccompiler.h"

KNGccCompiler::KNGccCompiler(QObject *parent) :
    KNCompiler(parent),
    m_gccPath(QString()),
    m_compileSwitchArgs(QStringList())
{
    //Initial the default switch args.
    m_defaultSwitchArgs << "-lm" <<"-ggdb" << "-Wall";
    //For windows, we should compile the file in static mode.
#ifdef Q_OS_WIN32
    m_defaultSwitchArgs << "-static";
#endif
}

QString KNGccCompiler::compilerPath()
{
    if(m_gccPath.isEmpty())
    {
#ifdef Q_OS_WIN32
#else
        //Default GCC path is /usr/bin/gcc for UNIX.
        return QString("/usr/bin/gcc");
#endif
    }
    return m_gccPath;
}

QStringList KNGccCompiler::getVersionArgs()
{
    return QStringList("-dumpversion");
}

QString KNGccCompiler::environmentsArgs()
{
#ifdef Q_OS_WIN32
    ;
#else
    return QString();
#endif
}

QStringList KNGccCompiler::getCompileArgs(const QString &filePath)
{
    //Generate the arguments string list, start with the file path.
    QStringList args(filePath);
    //Add compiler switch args.
    //If the custom args is empty, add default args.
    args.append(compileSwitchArgs().isEmpty()?
                    defaultSwitchArgs():compileSwitchArgs());
    //Add output file path.
    QFileInfo sourceFileInfo(filePath);
    QString executeFilePath=
            sourceFileInfo.absoluteDir().filePath(sourceFileInfo.completeBaseName());
#ifdef Q_OS_WIN32
    executeFilePath.append(".exe");
#endif
    args << "-o" << executeFilePath;
    return args;
}

QStringList KNGccCompiler::compileSwitchArgs()
{
    return m_compileSwitchArgs;
}

QStringList KNGccCompiler::defaultSwitchArgs()
{
    return m_defaultSwitchArgs;
}
