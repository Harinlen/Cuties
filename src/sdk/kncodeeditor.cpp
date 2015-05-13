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
#include <QBoxLayout>

#include "knglobal.h"
#include "kntextedit.h"
#include "knlanguagemode.h"
#include "knhighlighter.h"

#include "kncodeeditor.h"

KNCodeEditor::KNCodeEditor(QWidget *parent) :
    QWidget(parent),
    m_editor(new KNTextEdit(this)),
    m_languageMode(nullptr)
{
    setObjectName("CodeEditor");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    mainLayout->addWidget(m_editor, 1);

    //Set language mode.
    setLanguageMode(KNGlobal::instance()->getLanguageMode("cpp"));
}

KNCodeEditor::~KNCodeEditor()
{
    //Clear the language mode and recover the memory.
    clearLanguageMode();
}

KNLanguageMode *KNCodeEditor::languageMode() const
{
    return m_languageMode;
}

void KNCodeEditor::setLanguageMode(KNLanguageMode *languageMode)
{
    //Clear the old language mode.
    clearLanguageMode();
    //Set new language mode.
    m_languageMode = languageMode;
    //Link the language mode.
    m_languageMode->highlighter()->setDocument(m_editor->document());
}

inline void KNCodeEditor::clearLanguageMode()
{
    if(m_languageMode)
    {
        //Delete the language mode.
        delete m_languageMode;
        m_languageMode=nullptr;
    }
}
