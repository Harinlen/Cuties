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

#ifndef KNTEXTEDITPANEL_H
#define KNTEXTEDITPANEL_H

#include <QTextBlock>

#include <QWidget>

class KNTextBlockData;
class KNTextEdit;
class KNTextEditPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNTextEditPanel(QWidget *parent = 0);

signals:
    void requireResizeMargin();

public slots:
    void setEditor(KNTextEdit *editor);

protected:
    virtual void drawContent(QRect blockRect,
                             const QTextBlock &block,
                             QPainter *painter,
                             bool currentLine)=0;
    virtual void itemClickEvent(const QTextBlock &block,
                                KNTextBlockData *data,
                                KNTextEdit *editor);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    virtual void onActionBlockCountChange(int blockCount);

private:
    bool m_pressed;
    KNTextEdit *m_editor;
    QTextBlock m_firstBlock, m_lastBlock;
};

#endif // KNTEXTEDITPANEL_H
