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

#ifndef KNTABMANAGER_H
#define KNTABMANAGER_H

#include <QList>

#include <QScrollArea>

class QSignalMapper;
class QBoxLayout;
class KNSidebar;
class KNTabManagerItem;
class KNSideShadowWidget;
class KNTabManagerContent;
class KNTabManager : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNTabManager(QWidget *parent = 0);
    void setSidebar(KNSidebar *sidebar);
    QWidget *contentWidget();

signals:

public slots:
    void addTab(const QString &caption);
    void setCurrentIndex(int index);
    void setCurrentItem(KNTabManagerItem *item);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionItemClicked();
    void onActionVerticalValueChanged(const int &value);

private:
    enum TabManagerActions
    {
        New,
        Open,
        Save,
        SaveAs,
        SaveAll,
        Close,
        CloseAll,
        CloseAllOthers,
        TabManagerActionCount
    };
    inline void initialActions();
    QAction *m_actions[TabManagerActionCount];

    QBoxLayout *m_containerLayout;
    QWidget *m_container;
    QSignalMapper *m_itemMapper;
    QList<KNTabManagerItem *> m_itemList;
    KNSideShadowWidget *m_topShadow, *m_bottomShadow;

    KNTabManagerContent *m_content;
    KNTabManagerItem *m_currentItem;
};

#endif // KNTABMANAGER_H
