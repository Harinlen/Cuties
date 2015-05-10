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

#ifndef KNTEXTEDITMARKPANEL_H
#define KNTEXTEDITMARKPANEL_H

#include <QPixmap>

#include "kntexteditpanel.h"

class KNTextEditMarkPanel : public KNTextEditPanel
{
    Q_OBJECT
public:
    explicit KNTextEditMarkPanel(QWidget *parent = 0);

signals:

public slots:

protected:
    void drawContent(QRect blockRect,
                     const QTextBlock &block,
                     QPainter *painter,
                     bool currentLine);
    void itemClickEvent(const QTextBlock &block,
                        KNTextBlockData *data);

private:
    QPixmap m_mark;
};

#endif // KNTEXTEDITMARKPANEL_H
