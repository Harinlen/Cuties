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

#ifndef KNWELCOMEBASE_H
#define KNWELCOMEBASE_H

#include <QWidget>

class KNWelcomeBase : public QWidget
{
    Q_OBJECT
public:
    KNWelcomeBase(QWidget *parent = 0):QWidget(parent){}

signals:
    void requireNewFile(const QString &suffix);
    void requireOpenFile(const QString &filePath);

public slots:

};

#endif // KNWELCOMEBASE_H
