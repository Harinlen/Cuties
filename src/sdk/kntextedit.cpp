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

#include <QScrollBar>

#include "../plugins/kntexteditmarkpanel/kntexteditmarkpanel.h"
#include "../plugins/kntexteditnumberpanel/kntexteditnumberpanel.h"
#include "knglobal.h"
#include "knsideshadowwidget.h"
#include "kntexteditpanelcontainer.h"

#include "kntextedit.h"

KNTextEdit::KNTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    m_panelContainer(new KNTextEditPanelContainer(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadow::TopShadow, this)),
    m_lineColor(QColor(0x30,0x30,0x30)),
    m_tabWidth(4)
{
    setObjectName("TextEditor");
    //Set palette.
    setFrameStyle(QFrame::NoFrame);
    setPalette(KNGlobal::instance()->getPalette(objectName()));

    //Set the default coding font: Monaco
    QFont codeFont=font();
    codeFont.setFamily("Monaco");
    setFont(codeFont);

    //Link request.
    connect(this, &KNTextEdit::cursorPositionChanged,
            this, &KNTextEdit::updateHighlights);

    //Configure the shadow.
    m_topShadow->resize(width(), 15);
    //Configure panel container.
    m_panelContainer->setEditor(this);
    connect(this, &KNTextEdit::textChanged,
            m_panelContainer, &KNTextEditPanelContainer::updatePanels);
    connect(this, &KNTextEdit::cursorPositionChanged,
            m_panelContainer, &KNTextEditPanelContainer::updatePanels);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            m_panelContainer, SLOT(updatePanels()));
    connect(m_panelContainer, &KNTextEditPanelContainer::requireResizeMargin,
            this, &KNTextEdit::onActionUpdateMargins);
    //Add panels.
    m_panelContainer->addPanel(new KNTextEditMarkPanel);
    m_panelContainer->addPanel(new KNTextEditNumberPanel);

    //Update the highlight for initialize.
    updateHighlights();
    //Emit all changed signal for initialize.
    ;
}

void KNTextEdit::setFont(const QFont &font)
{
    //Set the font.
    QPlainTextEdit::setFont(font);
    //Get the fontmetrics of the font.
    QFontMetrics metrics=QFontMetrics(font);
    setTabStopWidth(m_tabWidth*metrics.width(' '));
}

void KNTextEdit::setTabWidth(const int &spaceNum)
{
    //Save the tab width.
    m_tabWidth=spaceNum;
    //Update the tab stop.
    setTabStopWidth(fontMetrics().width(" ")*m_tabWidth);
}

void KNTextEdit::setOverwriteMode(bool overwrite)
{
    //Set the overwrite mode.
    QPlainTextEdit::setOverwriteMode(overwrite);
    //Emit the overwrite change signal.
    emit overwriteModeChanged(overwrite);
}

void KNTextEdit::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QPlainTextEdit::resizeEvent(event);
    //Resize the shadow.
    m_topShadow->resize(width(), m_topShadow->height());
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
