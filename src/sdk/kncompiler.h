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

#ifndef KNCOMPILER_H
#define KNCOMPILER_H

#include <QScopedPointer>
#include <QProcess>
#include <QStringList>
#include <QStandardItem>

#include <QObject>

class KNConnectionHandler;
class KNCompiler : public QObject
{
    Q_OBJECT
public:
    explicit KNCompiler(QObject *parent = 0);
    void compile(const QString &filePath);
    virtual QString compilerPath()=0;

signals:
    void compileMessageAppend(QString message);
    void compileItemAppend(QStandardItem *item);

public slots:

protected:
    virtual QStringList getVersionArgs()=0;
    virtual QStringList getCompileArgs(const QString &filePath)=0;
    virtual QString environmentsArgs()=0;
    virtual void onActionMessageAppend(QString message);
    virtual void onActionCompileFinished();
    QString filePath();

private slots:
    void onActionReadyForOutput();
    void onActionFinished(const int &exitCode,
                          QProcess::ExitStatus exitStatus);

private:
    QString m_filePath;
    QScopedPointer<QProcess> m_compilerProcess;
    KNConnectionHandler *m_compilerHandle;
};

#endif // KNCOMPILER_H
