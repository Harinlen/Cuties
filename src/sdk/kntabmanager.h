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
class KNCompileDockBase;
class KNTabManagerItem;
class KNSideShadowWidget;
class KNTabManagerContent;
class KNCodeEditorUnibar;
class KNTabManager : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNTabManager(QWidget *parent = 0);
    void setSidebar(KNSidebar *sidebar);
    QWidget *contentWidget();
    KNCodeEditorUnibar *unibar() const;
    void setUnibar(KNCodeEditorUnibar *unibar);
    KNCompileDockBase *compileDock() const;
    void setCompileDock(KNCompileDockBase *compileDock);

signals:

public slots:
    KNTabManagerItem *createTab(const QString &caption,
                                const QString &filePath="");
    void setCurrentIndex(int index);
    void setCurrentItem(KNTabManagerItem *item);
    void closeTab(KNTabManagerItem *item);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionItemClicked();
    void onActionVerticalValueChanged(const int &value);

    //Action slots.
    void onActionNewSourceFile();
    void onActionOpen();
    void onActionSave();
    void onActionSaveAs();
    void onActionSaveAll();
    void onActionClose();
    void onActionCloseAll();
    void onActionCloseAllOthers();

    void onActionCompile();

private:
    inline void saveItem(KNTabManagerItem *item);
    inline void saveAsItem(KNTabManagerItem *item);
    inline void removeItem(KNTabManagerItem *item);
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
        Compile,
        TabManagerActionCount
    };
    inline void initialActions();
    QAction *m_actions[TabManagerActionCount];

    KNCodeEditorUnibar *m_unibar;
    QBoxLayout *m_containerLayout;
    QWidget *m_container;
    QSignalMapper *m_itemMapper;
    QList<KNTabManagerItem *> m_itemList;
    KNSideShadowWidget *m_topShadow, *m_bottomShadow;

    KNTabManagerContent *m_content;
    KNTabManagerItem *m_currentItem;
    KNCompileDockBase *m_compileDock;

    int m_untitledCounter;
    QString m_untitledPrefix;
};

#endif // KNTABMANAGER_H
