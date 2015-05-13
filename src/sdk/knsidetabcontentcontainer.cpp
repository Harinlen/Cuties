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
#include <QTimeLine>

#include "knglobal.h"

#include "knsidetabcontentcontainer.h"

KNSideTabContentContainer::KNSideTabContentContainer(QWidget *parent) :
    QWidget(parent),
    m_expandAnime(nullptr),
    m_widget(nullptr),
    m_sidebar(nullptr)
{
    setObjectName("SideTabContentContainer");
    //Set properties.
    setAutoFillBackground(true);

    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));

    //Configure time line.
    m_expandAnime=generateAnime(309);
    connect(m_expandAnime, &QTimeLine::finished,
            this, &KNSideTabContentContainer::onActionExpandFinished);
    m_foldAnime=generateAnime(0);
    connect(m_foldAnime, &QTimeLine::finished,
            this, &KNSideTabContentContainer::hide);
}

KNSideTabContentContainer::~KNSideTabContentContainer()
{

}

void KNSideTabContentContainer::expand()
{
    //Show the widget.
    show();
    //Start expand anime.
    startAnime(m_expandAnime);
}

void KNSideTabContentContainer::fold()
{
    //Hide the content first.
    m_widget->hide();
    //Start fold anime.
    startAnime(m_foldAnime);
}

void KNSideTabContentContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the content widget.
    if(m_widget && m_widget->isVisible())
    {
        m_widget->resize(size());
    }
}

void KNSideTabContentContainer::onActionExpandFinished()
{
    //Ensure the widget is not nullptr.
    if(m_widget==nullptr)
    {
        return;
    }
    //Resize the widget to current size.
    m_widget->resize(size());
    //Show the widget.
    m_widget->show();
}

void KNSideTabContentContainer::startAnime(QTimeLine *timeLine)
{
    //Stop all anime.
    m_expandAnime->stop();
    m_foldAnime->stop();
    //Configure anime.
    timeLine->setStartFrame(width());
    //Start anime.
    timeLine->start();
}

QTimeLine *KNSideTabContentContainer::generateAnime(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setUpdateInterval(16);
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            [=](const int &containerWidth){resize(containerWidth, height());});
    return timeLine;
}

void KNSideTabContentContainer::setSidebar(QWidget *sidebar)
{
    //Save the sidebar pointer.
    m_sidebar = sidebar;
}

QWidget *KNSideTabContentContainer::widget() const
{
    return m_widget;
}

void KNSideTabContentContainer::setWidget(QWidget *widget)
{
    //Check the previous widget is null or not.
    if(m_widget)
    {
        //Hide the widget.
        m_widget->hide();
        //Give the parent back to sidebar widget.
        m_widget->setParent(m_sidebar);
    }
    //Save the widget.
    m_widget = widget;
    if(m_widget==nullptr)
    {
        return;
    }
    //Change the parent ship.
    m_widget->setParent(this);
    //Check and resize the widget.
    if(isVisible())
    {
        m_widget->resize(size());
    }
}
