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

#ifndef KNSIDETABCONTENTCONTAINER_H
#define KNSIDETABCONTENTCONTAINER_H

#include <QWidget>

class QTimeLine;
class KNSideTabContentContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNSideTabContentContainer(QWidget *parent = 0);
    ~KNSideTabContentContainer();

    QWidget *widget() const;
    void setWidget(QWidget *widget);
    void setSidebar(QWidget *sidebar);

signals:

public slots:
    void expand();
    void fold();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionExpandFinished();

private:
    inline void startAnime(QTimeLine *timeLine);
    QTimeLine *generateAnime(const int &endFrame);

    QTimeLine *m_expandAnime, *m_foldAnime;
    QWidget *m_widget;
    QWidget *m_sidebar;
};

#endif // KNSIDETABCONTENTCONTAINER_H
