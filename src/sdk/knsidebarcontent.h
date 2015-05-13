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
#ifndef KNSIDEBARCONTENT_H
#define KNSIDEBARCONTENT_H

#include <QList>

#include <QWidget>

class KNLabelAnimeButton;
class QLabel;
class KNSidebarContent : public QWidget
{
    Q_OBJECT
public:
    explicit KNSidebarContent(QWidget *parent = 0);
    ~KNSidebarContent();
    void addTab(KNLabelAnimeButton *icon,
                QLabel *caption,
                QWidget *widget);

signals:
    void requireHideWidget();
    void requireShowWidget(QWidget *widget);

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    struct SidebarTab
    {
        KNLabelAnimeButton *icon;
        QLabel *caption;
        QWidget *widget;
    };
    inline void positionTab(const int &index, const SidebarTab &tab);
    QList<SidebarTab> m_tabList;

    const int m_foldIconSize=30, m_buttonMargin=3, m_contentMargin=3;
    int m_buttonSize;
    bool m_expand;
    int m_expandHeight;
    KNLabelAnimeButton *m_currentIcon;
};

#endif // KNSIDEBARCONTENT_H
