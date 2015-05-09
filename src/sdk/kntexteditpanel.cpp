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
#include <QPainter>
#include <QRectF>

#include "kntextedit.h"
#include "kntexteditpanel.h"
#include "knglobal.h"

#include <QDebug>

KNTextEditPanel::KNTextEditPanel(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("TextEditPanel");
    //Set properties.
    setContentsMargins(0,0,0,0);
    resize(10, height());
    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
}

void KNTextEditPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Here we can paint the panel.
    QTextBlock block=m_editor->firstVisibleBlock();
    QRectF translatedBlockRect;
    int panelBlockTop=0;
    while(block.isValid() && panelBlockTop<=event->rect().bottom())
    {
        //Draw the visible block.
        if(block.isVisible())
        {
            translatedBlockRect=
                        m_editor->blockBoundingGeometry(block).translated(
                            m_editor->contentOffset());
            panelBlockTop=translatedBlockRect.top();
            if(!block.next().isValid())
            {
                translatedBlockRect.setHeight(translatedBlockRect.height()-4);
            }
            int blockHeight=translatedBlockRect.height();
            drawContent(0,
                        panelBlockTop,
                        width(),
                        blockHeight,
                        block,
                        &painter,
                        m_editor->textCursor().blockNumber()==block.blockNumber());
            //Change the parameters.
            panelBlockTop+=blockHeight;
        }
        //Switch to the next block.
        block=block.next();
    }
}

void KNTextEditPanel::setEditor(KNTextEdit *editor)
{
    m_editor = editor;
    connect(m_editor, &KNTextEdit::blockCountChanged,
            this, &KNTextEditPanel::onActionBlockCountChange);
}

void KNTextEditPanel::onActionBlockCountChange(int blockCount)
{
    Q_UNUSED(blockCount)
    //Do nothing for default.
}

