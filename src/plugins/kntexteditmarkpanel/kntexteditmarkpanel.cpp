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

#include "kntextblockdata.h"
#include "knglobal.h"

#include "kntexteditmarkpanel.h"

KNTextEditMarkPanel::KNTextEditMarkPanel(QWidget *parent) :
    KNTextEditPanel(parent),
    m_mark(QPixmap(":/image/resource/images/BreakPoint.png"))
{
    setObjectName("TextEditMarkPanel");
    //Set properties.
    setFixedWidth(m_mark.width());

    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
}

void KNTextEditMarkPanel::drawContent(QRect blockRect,
                                      const QTextBlock &block,
                                      QPainter *painter,
                                      bool currentLine)
{
    //Draw the highlight if the block is the current line.
    if(currentLine)
    {
        painter->fillRect(blockRect, palette().highlight());
    }
    //Ignore no user data line.
    if(nullptr==block.userData())
    {
        return;
    }
    //If this line is marked, then draw the mark.
    KNTextBlockData *blockData=static_cast<KNTextBlockData *>(block.userData());
    if(blockData->marked)
    {
        painter->drawPixmap(blockRect, m_mark);
    }
}

void KNTextEditMarkPanel::itemClickEvent(const QTextBlock &block,
                                         KNTextBlockData *data)
{
    Q_UNUSED(block)
    //Set marked to the other side.
    data->marked^=1;
    //Update the panel.
    update();
}