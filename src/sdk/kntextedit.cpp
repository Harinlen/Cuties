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

#include "../plugins/kntexteditnumberpanel/kntexteditnumberpanel.h"

#include "knglobal.h"
#include "kntexteditpanelcontainer.h"

#include "kntextedit.h"

KNTextEdit::KNTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    m_panelContainer(new KNTextEditPanelContainer(this)),
    m_lineColor(QColor(0x30,0x30,0x30))
{
    setObjectName("TextEditor");
    //Set palette.
    setFrameStyle(QFrame::NoFrame);
    setPalette(KNGlobal::instance()->getPalette(objectName()));

    //Link request.
    connect(this, &KNTextEdit::cursorPositionChanged,
            this, &KNTextEdit::updateHighlights);

    //Configure panel container.
    m_panelContainer->setEditor(this);
    connect(this, &KNTextEdit::textChanged,
            m_panelContainer, &KNTextEditPanelContainer::updatePanels);
    connect(this, &KNTextEdit::cursorPositionChanged,
            m_panelContainer, &KNTextEditPanelContainer::updatePanels);
    connect(m_panelContainer, &KNTextEditPanelContainer::requireResizeMargin,
            this, &KNTextEdit::onActionUpdateMargins);
    //Add panels.
    m_panelContainer->addPanel(new KNTextEditNumberPanel);

    //Update the highlight for initialize.
    updateHighlights();
    //Emit all changed signal for initialize.
}

void KNTextEdit::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QPlainTextEdit::resizeEvent(event);
    //Resize the panel container.
    QRect cr=contentsRect();
    m_panelContainer->setGeometry(cr.left(),
                                  cr.top(),
                                  m_panelContainer->panelWidth(),
                                  cr.height());
}

void KNTextEdit::onActionUpdateMargins()
{
    setViewportMargins(m_panelContainer->panelWidth(),
                       0,
                       0,
                       0);
}

void KNTextEdit::updateHighlights()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    //Get extra selections.
    highlightCurrentLine(extraSelections);

    //Set the selections.
    setExtraSelections(extraSelections);
}

inline void KNTextEdit::highlightCurrentLine(QList<QTextEdit::ExtraSelection> &selections)
{
    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(m_lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor=textCursor();
        selection.cursor.clearSelection();
        selections.append(selection);
    }
}
