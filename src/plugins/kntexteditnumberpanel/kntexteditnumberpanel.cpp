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

#include "knglobal.h"
#include "kntextedit.h"

#include "kntexteditnumberpanel.h"

KNTextEditNumberPanel::KNTextEditNumberPanel(QWidget *parent) :
    KNTextEditPanel(parent)
{
    setObjectName("TextEditNumberPanel");
    //Set properties.
    setMinimumWidth(30);
    //Set the palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
}

void KNTextEditNumberPanel::drawContent(QRect blockRect,
                                        const QTextBlock &block,
                                        QPainter *painter,
                                        bool currentLine)
{
    QPen painterPen=painter->pen();
    //Draw the highlight if the block is the current line.
    if(currentLine)
    {
        painter->fillRect(blockRect, palette().highlight());
        painter->setPen(QColor(255,255,255));
    }
    //Draw the block number.
    painter->drawText(blockRect,
                      Qt::AlignVCenter | Qt::AlignRight,
                      QString::number(block.blockNumber()+1));
    painter->setPen(painterPen);
}

void KNTextEditNumberPanel::itemClickEvent(const QTextBlock &block,
                                           KNTextBlockData *data,
                                           KNTextEdit *editor)
{
    Q_UNUSED(data);
    //Generate the selection text cursor.
    QTextCursor cursor=editor->textCursor();
    cursor.setPosition(block.position());
    cursor.movePosition(QTextCursor::NextCharacter,
                        QTextCursor::KeepAnchor,
                        block.length()-1);
    //Set the selection cursor.
    editor->setTextCursor(cursor);
}

void KNTextEditNumberPanel::onActionBlockCountChange(int blockCount)
{
    //Resize the panel.
    resize(fontMetrics().width(QString::number(blockCount)), height());
    //Emit update signal.
    emit requireResizeMargin();
}
