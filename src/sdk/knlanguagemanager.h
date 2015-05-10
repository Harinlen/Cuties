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

#ifndef KNLANGUAGEMANAGER_H
#define KNLANGUAGEMANAGER_H

#include <QMap>

#include <QObject>

/*
 * A language should contains things below:
 * 1. Highlighter.
 * 2. Compiler.
 * 3. Debugger.
 * The language manager will generate a class named KNLanguageMode, and set the
 * language mode to KCCodeEditor, and the editor will link those contents.
 */

class KNLanguageMode;
class KNLanguageManager : public QObject
{
    Q_OBJECT
public:
    static KNLanguageManager *instance();
    KNLanguageMode *getLanguageMode(const QString &suffix);

signals:

public slots:

private:
    static KNLanguageManager *m_instance;
    explicit KNLanguageManager(QObject *parent = 0);
};

#endif // KNLANGUAGEMANAGER_H
