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
#include <QLabel>

#include "kntextedit.h"
#include "knglobal.h"
#include "knconnectionhandler.h"

#include "kncodeeditorunibar.h"

KNCodeEditorUnibar::KNCodeEditorUnibar(QWidget *parent) :
    QWidget(parent),
    m_editorConections(new KNConnectionHandler(this)),
    m_editor(nullptr),
    m_cursorPosition(new QLabel(this))
{
    setObjectName("CodeEditorUnibar");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(25);
    setContentsMargins(0,0,0,0);

    //Set palette.
    QPalette pal=KNGlobal::instance()->getPalette(objectName());
    QColor bgColor=pal.color(QPalette::Window);
    QLinearGradient bgBrush(0,0,0,25);
    bgColor.setHsv(bgColor.hue(), bgColor.saturation(), 129);
    bgBrush.setColorAt(0, bgColor);
    bgColor.setHsv(bgColor.hue(), bgColor.saturation(), 88);
    bgBrush.setColorAt(1, bgColor);
    pal.setBrush(QPalette::Window, bgBrush);
    setPalette(pal);
    //Initial layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    mainLayout->addWidget(m_cursorPosition);

    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNCodeEditorUnibar::retranslate);
    retranslate();
}

KNTextEdit *KNCodeEditorUnibar::editor() const
{
    return m_editor;
}

void KNCodeEditorUnibar::setEditor(KNTextEdit *editor)
{
    //Disconnect with the old connections.
    if(m_editor!=nullptr)
    {
        m_editorConections->disconnectAll();
    }
    //Set the editor.
    m_editor = editor;
    //Link with the new editor.
    m_editorConections->append(
                connect(m_editor, &KNTextEdit::cursorPositionChanged,
                        this, &KNCodeEditorUnibar::onActionCursorPositionChange));

    //Sync the data with the new editor.
    onActionCursorPositionChange();
}

void KNCodeEditorUnibar::retranslate()
{
    m_positionText=tr("Line %1, Row %2");
    onActionCursorPositionChange();
}

void KNCodeEditorUnibar::onActionCursorPositionChange()
{
    //Ignore illegal request.
    if(m_editor==nullptr)
    {
        return;
    }
    m_cursorPosition->setText(m_positionText.arg(QString::number(m_editor->textCursor().blockNumber()+1),
                                                 QString::number(m_editor->textCursor().positionInBlock()+1)));
}

