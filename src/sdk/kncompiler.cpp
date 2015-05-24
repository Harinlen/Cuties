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
#include "knconnectionhandler.h"

#include "kncompiler.h"

#include <QDebug>

KNCompiler::KNCompiler(QObject *parent) :
    QObject(parent),
    m_compilerHandle(new KNConnectionHandler(this))
{
}

void KNCompiler::compile(const QString &filePath)
{
    //Check the compiler handle first.
    if(!m_compilerHandle->isEmpty())
    {
        m_compilerHandle->disconnectAll();
    }
    //Reset the compiler process.
    m_compilerProcess.reset(new QProcess(this));
    //Copy from F1.
    m_compilerProcess->setProcessChannelMode(QProcess::MergedChannels);

    //Link the process.
    m_compilerHandle->append(
                connect(m_compilerProcess.data(), &QProcess::readyRead,
                        this, &KNCompiler::onActionReadyForOutput));
    //Signal 'finish()' is such a fuck func.
    m_compilerHandle->append(
                connect(m_compilerProcess.data(), SIGNAL(finished(int,QProcess::ExitStatus)),
                        this, SLOT(onActionFinished(int,QProcess::ExitStatus))));

    //Get the environment variables, add to system environment variables.
    QString pathVariable=environmentsArgs();
    if(!pathVariable.isEmpty())
    {
        //Get the system variables.
        QStringList compilerEnvironment=QProcess::systemEnvironment();
        //Add the PATH environment variable to the end of PATH.
        compilerEnvironment.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive),
                                             QString("PATH=\\1;"+pathVariable));
        m_compilerProcess->setEnvironment(compilerEnvironment);
    }

    //Start compile process.
    m_compilerProcess->start(compilerPath(),
                             getCompileArgs(filePath));
}

void KNCompiler::onActionReadyForOutput()
{
    qDebug()<<m_compilerProcess->readAll();
}

void KNCompiler::onActionFinished(const int &exitCode,
                                  QProcess::ExitStatus exitStatus)
{
    //Disconnect all links.
    m_compilerHandle->disconnectAll();
    //Emit finished data.
}
