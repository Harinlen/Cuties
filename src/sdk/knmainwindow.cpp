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
#include <QPropertyAnimation>

#include "knwelcomebase.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_welcome(nullptr),
    m_tabManager(nullptr),
    m_welcomeIn(generateAnime()),
    m_welcomeOut(generateAnime())
{
    setObjectName("MainWindow");
    //Set properties.
    setMinimumSize(950, 600);
}

KNWelcomeBase *KNMainWindow::welcome() const
{
    return m_welcome;
}

void KNMainWindow::setWelcome(KNWelcomeBase *welcome)
{
    //Save the welcome pointer.
    m_welcome = welcome;
    //Check the welcome is null or not.
    if(m_welcome==nullptr)
    {
        return;
    }
    //Configure the welcome widget.
    m_welcome->setParent(this);
    //Set the welcome window target.
    m_welcomeIn->setTargetObject(m_welcome);
    m_welcomeOut->setTargetObject(m_welcome);
    //Link the welcome window.
    connect(m_welcomeOut, &QPropertyAnimation::finished,
            [=]{m_welcome->hide();});
    connect(m_welcome, &KNWelcomeBase::requireNewFile,
            this, &KNMainWindow::onActionNewFile);
}

void KNMainWindow::setTabManager(QDockWidget *widget)
{
    //Save the tab manager.
    m_tabManager=widget;
    //Add the tab manager to widget.
    addDockWidget(Qt::LeftDockWidgetArea, m_tabManager);
}

void KNMainWindow::resizeEvent(QResizeEvent *event)
{
    //Resize the main window.
    QMainWindow::resizeEvent(event);
    //Check the welcome is visible or not.
    if(m_welcome && m_welcome->isVisible())
    {
        m_welcome->move((width()-m_welcome->width())>>1,
                        (height()-m_welcome->height())>>1);
    }
}

void KNMainWindow::onActionNewFile(const QString &suffix)
{
    //Hide the welcome window.
    m_welcomeOut->setStartValue(m_welcome->pos());
    m_welcomeOut->setEndValue(QPoint(m_welcome->x(),
                                     -5-m_welcome->height()));
    m_welcomeOut->start();
}

inline QPropertyAnimation *KNMainWindow::generateAnime()
{
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    anime->setPropertyName("pos");
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setDuration(200);
    return anime;
}
