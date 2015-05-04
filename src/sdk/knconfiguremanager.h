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

#ifndef KNCONFIGUREMANAGER_H
#define KNCONFIGUREMANAGER_H

#include <QObject>

namespace KNConfigureManagerGlobal
{
enum ConfigureTypes
{
    Cache,
    System,
    User,
    ConfigureTypeCount
};
}

using namespace KNConfigureManagerGlobal;

class KNConfigure;
class KNConfigureManager : public QObject
{
    Q_OBJECT
public:
    static KNConfigureManager *instance();
    void loadConfigure();
    void saveConfigure();

signals:

public slots:
    void setConfigureDir(const QString &dirPath);

private:
    static KNConfigureManager *m_instance;
    explicit KNConfigureManager(QObject *parent = 0);

    QString m_configureFileName[ConfigureTypeCount];
    KNConfigure *m_configure[ConfigureTypeCount];
};

#endif // KNCONFIGUREMANAGER_H
