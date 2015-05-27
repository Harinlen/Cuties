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

#ifndef KNSIDEBAR_H
#define KNSIDEBAR_H

#include "knglobal.h"

#include <QWidget>

class QTimeLine;
class QLabel;
class QPushButton;
class QScrollArea;
class KNLabelAnimeButton;
class KNSidebarContent;
class KNSAOSubMenu;
class KNMainMenu;
class KNSideTabContentContainer;
class KNSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit KNSidebar(QWidget *parent = 0);
    void addCategoryAction(int category, QAction *action);
    void addTab(KNLabelAnimeButton *icon,
                QLabel *caption,
                QWidget *widget);
    void setTabContentContainer(KNSideTabContentContainer *container);

signals:

public slots:
    void expand();
    void fold();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionShowMainMenu();
    void onActionShowWidget(QWidget *widget);

private:
    inline void startResizeAnime(const int &targetWidth);
    QPushButton *m_mainMenuButton;
    KNMainMenu *m_mainMenu;
    QAction *m_exitAction;
    QScrollArea *m_sidebarArea;
    KNSidebarContent *m_sidebarContent;
    QTimeLine *m_expandFoldAnime;
    KNSideTabContentContainer *m_tabContentContainer;

    int m_foldWidth, m_expandWidth;

    KNSAOSubMenu *m_categories[MainMenuCategoryCount];
};

#endif // KNSIDEBAR_H
