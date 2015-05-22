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

#ifndef KNMAINWINDOW_H
#define KNMAINWINDOW_H

#include <QMainWindow>

class QPropertyAnimation;
class QBoxLayout;
class QLabel;
class KNSidebar;
class KNTabManager;
class KNLabelAnimeButton;
class KNWelcomeBase;
class KNSideTabContentContainer;
class KNCodeEditorUnibar;
class KNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit KNMainWindow(QWidget *parent = 0);
    KNWelcomeBase *welcome() const;
    void addSidebarElement();

signals:

public slots:
    void setWelcome(KNWelcomeBase *welcome);
    void setUnibar(KNCodeEditorUnibar *widget);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionNewFile(const QString &suffix);

private:
    inline QPropertyAnimation *generateAnime();
    KNLabelAnimeButton *m_expandSidebar;
    KNSidebar *m_sidebar;
    KNWelcomeBase *m_welcome;
    KNTabManager *m_tabManager;
    KNLabelAnimeButton *m_tabSideButton;
    QLabel *m_tabSideCaption;
    KNSideTabContentContainer *m_tabContentContainer;
    QBoxLayout *m_headerLayout;
    QPropertyAnimation *m_welcomeIn, *m_welcomeOut;
};

#endif // KNMAINWINDOW_H
