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

#ifndef KNTHEMEMANAGER_H
#define KNTHEMEMANAGER_H

#include <QHash>
#include <QMap>
#include <QPalette>

#include <QObject>

class QJsonObject;
class KNThemeManager : public QObject
{
    Q_OBJECT
public:
    static KNThemeManager *instance();
    QPalette getPalette(const QString &name) const;

signals:

public slots:
    void loadTheme(const QString &themeFilePath);

private:
    inline void parsePalette(const QString &name,
                             QJsonObject *data,
                             QHash<QString, QPalette> &map);
    inline QColor parseColor(const QString &data);

    static KNThemeManager *m_instance;
    explicit KNThemeManager(QObject *parent = 0);

    QMap<QString, QPalette::ColorRole> m_colorRoleMap;
    QHash<QString, QPalette> m_palMap;
};

#endif // KNTHEMEMANAGER_H
