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
#include <QFrame>

#include "kntextedit.h"
#include "knglobal.h"
#include "knlabelanimebutton.h"
#include "knconnectionhandler.h"

#include "kncodeeditorunibar.h"

KNCodeEditorUnibar::KNCodeEditorUnibar(QWidget *parent) :
    QWidget(parent),
    m_editorConections(new KNConnectionHandler(this)),
    m_editor(nullptr),
    m_fileName(new QLabel(this)),
    m_cursorPosition(new QLabel(this)),
    m_encoded(new QLabel(this)),
    m_close(new KNLabelAnimeButton(this)),
    m_borderColor(QColor(255,255,255,52))
{
    setObjectName("CodeEditorUnibar");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(26);
    setContentsMargins(0,0,0,0);

    //Set palette.
    QPalette unibarPal=KNGlobal::instance()->getPalette(objectName());
    setPalette(unibarPal);
    //Initial layout.
    QBoxLayout *borderLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    borderLayout->setContentsMargins(0,0,0,0);
    borderLayout->setSpacing(0);
    setLayout(borderLayout);

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          borderLayout->widget());
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    borderLayout->addLayout(mainLayout, 1);
    QFrame *borderLine=new QFrame(this);
    QPalette pal=borderLine->palette();
    pal.setColor(QPalette::WindowText, m_borderColor);
    borderLine->setPalette(pal);
    borderLine->setContentsMargins(0,0,0,0);
    borderLine->setFrameStyle(QFrame::HLine);
    borderLayout->addWidget(borderLine);

    //Configure cursor widget.
    m_cursorPosition->setContentsMargins(9,0,9,0);
    //Configure encoded widget.
    m_encoded->setContentsMargins(9,0,9,0);
    //Configure close widget.
    m_close->setContentsMargins(0,0,0,0);
    m_close->setScaledContents(true);
    m_close->setFixedSize(25,25);
    m_close->setPixmap(QPixmap(":/image/resource/images/close.png"));

    mainLayout->addWidget(m_fileName, 1);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_cursorPosition);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_encoded);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_close);

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

QFrame *KNCodeEditorUnibar::generateSeperateLine()
{
    QFrame *line=new QFrame(this);
    line->setContentsMargins(0,0,0,0);
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, m_borderColor);
    line->setFrameStyle(QFrame::VLine);
    line->setPalette(pal);
    return line;
}

