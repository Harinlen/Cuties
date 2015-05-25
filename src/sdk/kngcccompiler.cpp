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
#include <QStandardItem>
#include <QDir>

#include "kncompileglobal.h"
#include "kngcccompiler.h"

#include <QDebug>

KNGccCompiler::KNGccCompiler(QObject *parent) :
    KNCompiler(parent),
    m_gccPath(QString()),
    m_compileSwitchArgs(QStringList()),
    m_cacheData(QString()),
    m_cacheItem(nullptr)
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

void KNGccCompiler::onActionMessageAppend(QString message)
{
    //Check the head of the item.
    if(message.left(filePath().length())==filePath())
    {
        //Check the message, remove the last '\n'.
        if(message.right(1)=="\n")
        {
            message.resize(message.size()-1);
        }
        //Check if there's previous of cache, emit the item.
        if(m_cacheItem!=nullptr)
        {
            //Remove the last '\n'.
            if((*(m_cacheData.end()-1))==QChar('\n'))
            {
                m_cacheData.resize(m_cacheData.size()-1);
            }
            //Set the cache data to cache item.
            m_cacheItem->setData(m_cacheData, Qt::UserRole+5);
            //Emit the item.
            emit compileItemAppend(m_cacheItem);
            //Reset the item.
            m_cacheItem=nullptr;
            m_cacheData.clear();
        }
        //Generate the item.
        QString compileInfoDetail=message.mid(filePath().length());
        //For the compileInfoDetail, it should be a string like this:
        //:9:9: warning: missing ...
        //1 2 3        4
        //So the things are:
        //  - Line: between the 1st and the 2nd colon.
        //  - Column: between the 2nd colon and the 3rd colon.
        //  - Error type: between the 3rd and the 4th colon, it should be
        //whether 'error' or 'warning'.
        //  - Detail: after the 4th colon.
        //Get the position of the colons.
        int secondColonPos=compileInfoDetail.indexOf(':', 1),
            thirdColonPos=compileInfoDetail.indexOf(':', secondColonPos+1),
            forthColonPos=compileInfoDetail.indexOf(':', thirdColonPos+1);
        QString detail=compileInfoDetail.mid(forthColonPos+1).simplified(),
                detailType=compileInfoDetail.mid(thirdColonPos+1, forthColonPos-thirdColonPos-1).simplified();
        m_cacheItem=new QStandardItem(detail);
        m_cacheItem->setEditable(false);
        //Add detail item to the cache item.
        if(detailType=="error")
        {
            m_cacheItem->setIcon(QIcon(KNCompileGlobal::instance()->errorIcon()));
        }
        else if(detailType=="warning")
        {
            m_cacheItem->setIcon(QIcon(KNCompileGlobal::instance()->warningIcon()));
        }
        m_cacheItem->setData(compileInfoDetail.mid(1, secondColonPos-1).toInt(),
                             Qt::UserRole+1);
        m_cacheItem->setData(compileInfoDetail.mid(secondColonPos+1, thirdColonPos-secondColonPos-1).toInt(),
                             Qt::UserRole+2);
        m_cacheItem->setData(detailType,
                             Qt::UserRole+3);
        m_cacheItem->setData(detail,
                             Qt::UserRole+4);
        return;
    }
    //Or else, it will be the sub item of the cache item.
    if(m_cacheItem!=nullptr)
    {
        m_cacheData.append("    " + message);
    }
}

void KNGccCompiler::onActionCompileFinished()
{
    //Check if there's previous of cache, emit the item.
    if(m_cacheItem!=nullptr)
    {
        //Remove the last '\n'.
        if((*(m_cacheData.end()-1))==QChar('\n'))
        {
            m_cacheData.resize(m_cacheData.size()-1);
        }
        //The last line of GCC output is the summary of the output data.
        //Seperate the last line.
        int lastLinePos=m_cacheData.lastIndexOf('\n');
        QStandardItem *summaryItem=nullptr;
        if(lastLinePos!=-1)
        {
            //Generate the summary item.
            summaryItem=new QStandardItem(m_cacheData.mid(lastLinePos+1).simplified());
            summaryItem->setData(-1, Qt::UserRole+1);
            //Remove the summary from the cache data.
            m_cacheData.resize(lastLinePos);
        }
        //Set the cache data to cache item.
        m_cacheItem->setData(m_cacheData, Qt::UserRole+5);
        //Emit the item.
        emit compileItemAppend(m_cacheItem);
        if(summaryItem!=nullptr)
        {
            emit compileItemAppend(summaryItem);
        }
        //Reset the item.
        m_cacheItem=nullptr;
        m_cacheData.clear();
    }
}

QStringList KNGccCompiler::compileSwitchArgs()
{
    return m_compileSwitchArgs;
}

QStringList KNGccCompiler::defaultSwitchArgs()
{
    return m_defaultSwitchArgs;
}
