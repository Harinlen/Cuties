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
#include <cstring>

#include "knconnectionhandler.h"

#include "kncompiler.h"

#include <QDebug>

#define OUTPUT_BUFFER_SIZE 512

KNCompiler::KNCompiler(QObject *parent) :
    QObject(parent),
    m_compilerHandle(new KNConnectionHandler(this))
{
}

void KNCompiler::compile(const QString &filePath)
{
    //-----Prepareing Compiler-----
    emit compileProgressChange(tr("Preparing Compiler"), PrepareCompiler);
    //Check the compiler handle first.
    if(!m_compilerHandle->isEmpty())
    {
        m_compilerHandle->disconnectAll();
    }
    //Save the file path.
    m_filePath=filePath;
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

    //-----Configuring the Compiler-----
    emit compileProgressChange(tr("Configuring the Compiler"), ConfiguringCompiler);
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

    //----Compiling----
    emit compileProgressChange(tr("Compiling"), Compiling);
    //Start compile process.
    m_compilerProcess->start(compilerPath(),
                             getCompileArgs(filePath));
}

void KNCompiler::onActionMessageAppend(QString message)
{
    Q_UNUSED(message);
}

void KNCompiler::onActionCompileFinished(const int &exitCode)
{
    Q_UNUSED(exitCode);
}

QString KNCompiler::filePath()
{
    return m_filePath;
}

void KNCompiler::onActionReadyForOutput()
{
    //Use OUTPUT_BUFFER_SIZE buffer to read data.
    char buffer[OUTPUT_BUFFER_SIZE];
    while(m_compilerProcess->readLine(buffer, OUTPUT_BUFFER_SIZE))
    {
        int bufferLength=strlen(buffer);
        //If the line is shorter than the buffer, the output the data.
        if(buffer[bufferLength-1]=='\n')
        {
            //Do message append action.
            onActionMessageAppend(buffer);
            //Emit message append signal.
            emit compileMessageAppend(buffer);
            continue;
        }
        //Else, genereate a message string, read until all the line readed.
        QString message;
        while(bufferLength>0 && buffer[bufferLength-1]!='\n')
        {
            //First add current buffer to the message.
            message.append(buffer);
            //Read the next buffer size data.
            m_compilerProcess->readLine(buffer, OUTPUT_BUFFER_SIZE);
            //Update the buffer length
            bufferLength=strlen(buffer);
        }
        //Append the last buffer data to string.
        message.append(buffer);
        //Do message append action.
        onActionMessageAppend(message);
        //Emit message append signal.
        emit compileMessageAppend(message);
    }
}

void KNCompiler::onActionFinished(const int &exitCode,
                                  QProcess::ExitStatus exitStatus)
{
    //Disconnect all links.
    m_compilerHandle->disconnectAll();
    //Get the exit status of the compiler progress, if normally exit, then call
    //the finished function.
    if(QProcess::NormalExit==exitStatus)
    {
        //Do compile finish action.
        onActionCompileFinished(exitCode);
        //Emit finished signal.
        emit compileFinished();
        return;
    }
    //Or else, emit compiler error.
    emit compileProgressChange(tr("Compiler Process Crash"), CompileFailed);
    emit compileFinished();
}
