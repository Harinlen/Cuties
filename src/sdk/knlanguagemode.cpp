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
#include "knlanguagemode.h"

KNLanguageMode::KNLanguageMode(QObject *parent) :
    QObject(parent),
    m_highlighter(nullptr),
    m_compiler(nullptr)
{
}

KNHighlighter *KNLanguageMode::highlighter()
{
    return m_highlighter;
}

void KNLanguageMode::setHighlighter(KNHighlighter *highlighter)
{
    m_highlighter = highlighter;
}

QString KNLanguageMode::languageName() const
{
    return m_languageName.isEmpty()?tr("Plain Text"):m_languageName;
}

void KNLanguageMode::setLanguageName(const QString &languageName)
{
    m_languageName = languageName;
}

KNCompiler *KNLanguageMode::compiler() const
{
    return m_compiler;
}

void KNLanguageMode::setCompiler(KNCompiler *compiler)
{
    m_compiler = compiler;
}
