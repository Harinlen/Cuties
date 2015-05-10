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

#ifndef KNCODESTYLEMANAGER_H
#define KNCODESTYLEMANAGER_H

#include <QMap>
#include <QTextCharFormat>

#include <QObject>

class KNCodeStyleManager : public QObject
{
    Q_OBJECT
public:
    static KNCodeStyleManager *instance();
    QTextCharFormat getFormat(const QString &name);

signals:

public slots:
    void loadCodeStyle(const QString &styleFilePath);

private:
    inline void parseStyle(const QString &ruleName,
                           const QString &ruleData,
                           QMap<QString, QTextCharFormat> &map);
    static KNCodeStyleManager *m_instance;
    explicit KNCodeStyleManager(QObject *parent = 0);

    QMap<QString, QTextCharFormat> m_codeStyle;
};

#endif // KNCODESTYLEMANAGER_H
