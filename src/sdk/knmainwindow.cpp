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
#include <QBoxLayout>
#include <QSplitter>

#include "knwelcomebase.h"
#include "kntabmanager.h"
#include "knsidebar.h"
#include "knsidetabcontentcontainer.h"
#include "kncompiledockbase.h"
#include "knlabelanimebutton.h"
#include "kncodeeditorunibar.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_expandSidebar(new KNLabelAnimeButton(this)),
    m_sidebar(new KNSidebar(this)),
    m_welcome(nullptr),
    m_tabManager(new KNTabManager),
    m_tabSideButton(new KNLabelAnimeButton(this)),
    m_tabSideCaption(new QLabel(this)),
    m_tabContentContainer(new KNSideTabContentContainer(this)),
    m_headerLayout(new QBoxLayout(QBoxLayout::TopToBottom)),
    m_welcomeIn(generateAnime()),
    m_welcomeOut(generateAnime()),
    m_topDockArea(new QSplitter(Qt::Horizontal, this))
{
    setObjectName("MainWindow");
    //Set properties.
    setMinimumSize(950, 600);

    //Set central widget and set layout.
    QWidget *container=new QWidget(this);
    setCentralWidget(container);
    QBoxLayout *centralLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             container);
    centralLayout->setContentsMargins(0,0,0,0);
    centralLayout->setSpacing(0);
    container->setLayout(centralLayout);

    //Add widget to central layout.
    centralLayout->addWidget(m_sidebar);

    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             centralLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);
    centralLayout->addLayout(contentLayout, 1);

    //Configure header layout.
    m_headerLayout->setParent(contentLayout->widget());
    m_headerLayout->setContentsMargins(0,0,0,0);
    m_headerLayout->setSpacing(0);

    contentLayout->addLayout(m_headerLayout);

    QSplitter *dockSplitter=new QSplitter(Qt::Vertical, this);
    dockSplitter->setContentsMargins(0,0,0,0);
    dockSplitter->setHandleWidth(0);
    contentLayout->addWidget(dockSplitter, 1);

    dockSplitter->addWidget(m_topDockArea);
    dockSplitter->addWidget(m_tabManager->contentWidget());

    //Configure sidebar button.
    m_expandSidebar->setPixmap(QPixmap(":/image/resource/images/expand.png"));
    //Configure sidebar tab content container.
    m_sidebar->setTabContentContainer(m_tabContentContainer);
    m_tabContentContainer->move(m_sidebar->width(), 0);
    m_tabContentContainer->resize(0, height());
    m_tabContentContainer->hide();

    //Add tab manager to sidebar.
    m_tabSideButton->setPixmap(QPixmap(":/image/resource/images/tabs.png"));
    m_sidebar->addTab(m_tabSideButton, m_tabSideCaption, m_tabManager);
    //Give the tab manager the main window to add actions.
    m_tabManager->setSidebar(m_sidebar);

    //Link retranslate.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNMainWindow::retranslate);
    retranslate();
}

KNWelcomeBase *KNMainWindow::welcome() const
{
    return m_welcome;
}

void KNMainWindow::addSidebarElement()
{
    ;
}

void KNMainWindow::addTopDockWidget(QWidget *widget)
{
    Q_ASSERT(widget!=nullptr);
    //Add the widget to dock area
    m_topDockArea->addWidget(widget);
}

void KNMainWindow::setWelcome(KNWelcomeBase *welcome)
{
    //Save the welcome pointer.
    m_welcome=welcome;
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

void KNMainWindow::setUnibar(KNCodeEditorUnibar *widget)
{
    widget->setSidebar(m_sidebar);
    //Set the unibar.
    m_tabManager->setUnibar(widget);
    //Add to header layout.
    m_headerLayout->addWidget(widget);
    //Raise the sidebar container.
    m_tabContentContainer->raise();
}

void KNMainWindow::setCompileDock(KNCompileDockBase *compileDock)
{
    //Add the compile dock to the top area.
    addTopDockWidget(compileDock);
    //Give the compile dock to tab manager.
    m_tabManager->setCompileDock(compileDock);
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
    //Resize the sidebar tab content container.
    //Keep the height sync to main window.
    m_tabContentContainer->resize(m_tabContentContainer->width(),
                                  height());
}

void KNMainWindow::retranslate()
{
    m_tabSideCaption->setText(tr("Opened Files"));
}

void KNMainWindow::onActionNewFile(const QString &suffix)
{
    Q_UNUSED(suffix)
    //Hide the welcome window.
    m_welcomeOut->setStartValue(m_welcome->pos());
    m_welcomeOut->setEndValue(QPoint(m_welcome->x(),
                                     -5-m_welcome->height()));
    m_welcomeOut->start();
    //Show all the widget.
    ;
}

inline QPropertyAnimation *KNMainWindow::generateAnime()
{
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    anime->setPropertyName("pos");
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setDuration(200);
    return anime;
}
