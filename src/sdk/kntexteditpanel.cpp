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
#include <QMouseEvent>

#include "kntextedit.h"
#include "kntexteditpanel.h"
#include "kntextblockdata.h"
#include "knglobal.h"

#include <QDebug>

KNTextEditPanel::KNTextEditPanel(QWidget *parent) :
    QWidget(parent),
    m_pressed(false),
    m_firstBlock(QTextBlock())
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
    m_firstBlock=m_editor->firstVisibleBlock();
    QTextBlock block=m_firstBlock;
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
            //Get the block rect.
            QRect blockRect=QRect(0,panelBlockTop,width(),blockHeight);
            //Save the block rect.
            KNTextBlockData *blockData=static_cast<KNTextBlockData *>(block.userData());
            if(nullptr!=blockData)
            {
                blockData->blockBorder=blockRect;
            }
            //Draw the panel.
            drawContent(blockRect,
                        block,
                        &painter,
                        m_editor->textCursor().blockNumber()==block.blockNumber());
            //Change the parameters.
            panelBlockTop+=blockHeight;
        }
        //Switch to the next block.
        block=block.next();
    }
    m_lastBlock=block;
}

void KNTextEditPanel::mousePressEvent(QMouseEvent *event)
{
    //Check and save the pressed event.
    if(event->buttons() == Qt::LeftButton)
    {
        m_pressed=true;
    }
    QWidget::mousePressEvent(event);
}

void KNTextEditPanel::mouseReleaseEvent(QMouseEvent *event)
{
    //Check the pressed event.
    if(m_pressed)
    {
        QTextBlock block=m_firstBlock;
        while(block.isValid() && block!=m_lastBlock)
        {
            //Current block must be a visible one.
            if(!block.isVisible())
            {
                block=block.next();
                continue;
            }
            //Get the border of the block.
            KNTextBlockData *data=
                    static_cast<KNTextBlockData *>(block.userData());
            //Check the border contains the pos or not. If so, do click event.
            if(nullptr!=data && data->blockBorder.contains(event->pos()))
            {
                itemClickEvent(block, data, m_editor);
                break;
            }
            //Switch to next block.
            block=block.next();
        }
    }
    //Reset the pressed flag.
    m_pressed=false;
    QWidget::mouseReleaseEvent(event);
}

void KNTextEditPanel::setEditor(KNTextEdit *editor)
{
    m_editor = editor;
    connect(m_editor, &KNTextEdit::blockCountChanged,
            this, &KNTextEditPanel::onActionBlockCountChange);
}

void KNTextEditPanel::itemClickEvent(const QTextBlock &block,
                                     KNTextBlockData *data,
                                     KNTextEdit *editor)
{
    Q_UNUSED(block);
    Q_UNUSED(data);
    Q_UNUSED(editor);
    //Do nothing for default.
}

void KNTextEditPanel::onActionBlockCountChange(int blockCount)
{
    Q_UNUSED(blockCount)
    //Do nothing for default.
}

