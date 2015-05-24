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

#ifndef KNLANGUAGEMODE_H
#define KNLANGUAGEMODE_H

#include <QObject>

class KNHighlighter;
class KNCompiler;
class KNLanguageMode : public QObject
{
    Q_OBJECT
public:
    explicit KNLanguageMode(QObject *parent = 0);

    KNHighlighter *highlighter();
    void setHighlighter(KNHighlighter *highlighter);

    QString languageName() const;
    void setLanguageName(const QString &languageName);

    KNCompiler *compiler() const;
    void setCompiler(KNCompiler *compiler);

private:
    KNHighlighter *m_highlighter;
    KNCompiler *m_compiler;
    QString m_languageName;
};

#endif // KNLANGUAGEMODE_H
