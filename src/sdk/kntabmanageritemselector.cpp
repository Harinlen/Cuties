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
#include "kntabmanageritem.h"

#include "kntabmanageritemselector.h"

KNTabManagerItemSelector::KNTabManagerItemSelector(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("TabManagerItemSelector");
    //Set properties.
    int selectorHeight=KNTabManagerItem::itemHeight(),
        selectorWidth=0.618*selectorHeight;
    setFixedSize(selectorWidth, selectorHeight);

    //Initial the border.
    m_selectorBorder << QPoint(selectorWidth, 0)
                     << QPoint(0, selectorHeight>>1)
                     << QPoint(selectorWidth, selectorHeight);
    //Update the background.
    updateBackground();
}

void KNTabManagerItemSelector::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Draw the selector.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawPolygon(m_selectorBorder);
}

void KNTabManagerItemSelector::updateBackground()
{
    m_backgroundColor=KNGlobal::instance()->getPalette("CodeEditor").color(QPalette::Window);
}
