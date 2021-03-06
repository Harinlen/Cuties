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

#ifndef KNGCCCOMPILER_H
#define KNGCCCOMPILER_H

#include "kncompiler.h"

class KNGccCompiler : public KNCompiler
{
    Q_OBJECT
public:
    explicit KNGccCompiler(QObject *parent = 0);
    QString compilerPath();

signals:

public slots:

protected:
    QStringList getVersionArgs();
    QString environmentsArgs();
    QStringList getCompileArgs(const QString &filePath);
    void onActionMessageAppend(QString message);
    void onActionCompileFinished(const int &exitCode);
    virtual QStringList compileSwitchArgs();
    virtual QStringList defaultSwitchArgs();

private:
    QString m_gccPath;
    QStringList m_compileSwitchArgs, m_defaultSwitchArgs;
    QString m_cacheData;
    QStandardItem *m_cacheItem;
};

#endif // KNGCCCOMPILER_H
