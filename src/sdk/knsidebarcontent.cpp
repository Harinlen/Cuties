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
#include "knglobal.h"
#include "knlabelanimebutton.h"

#include "knsidebarcontent.h"

#include <QDebug>

KNSidebarContent::KNSidebarContent(QWidget *parent) :
    QWidget(parent),
    m_buttonSize(m_foldIconSize-(m_buttonMargin<<1)),
    m_expand(false),
    m_expandHeight(0),
    m_currentIcon(nullptr)
{
    setObjectName("SidebarContent");
    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
}

KNSidebarContent::~KNSidebarContent()
{
    ;
}

void KNSidebarContent::addTab(KNLabelAnimeButton *icon,
                              QLabel *caption,
                              QWidget *widget)
{
    //Configure icon.
    icon->setPalette(KNGlobal::instance()->getPalette("SidebarButton"));
    icon->setParent(this);
    icon->setScaledContents(true);
    icon->setCheckable(true);
    icon->setContentsMargins(m_contentMargin,
                             m_contentMargin,
                             m_contentMargin,
                             m_contentMargin);
    icon->setGeometry(m_buttonMargin,
                      m_foldIconSize*m_tabList.size()+m_buttonMargin,
                      m_buttonSize,
                      m_buttonSize);
    connect(icon, &KNLabelAnimeButton::clicked,
            [=]
            {
                //Check if the current icon is nullptr.
                if(m_currentIcon)
                {
                    //Clear the clicked state of the icon.
                    m_currentIcon->setChecked(false);
                    //Configure the button
                    m_currentIcon->setContentsMargins(m_contentMargin,
                                                      m_contentMargin,
                                                      m_contentMargin,
                                                      m_contentMargin);
                    m_currentIcon->resize(m_buttonSize, m_buttonSize);
                    //If the current icon is myself, means to hide the widget.
                    if(m_currentIcon==icon)
                    {
                        emit requireHideWidget();
                        //Clear the current icon pointer.
                        m_currentIcon=nullptr;
                        return;
                    }
                }
                //Set the current icon.
                m_currentIcon=icon;
                m_currentIcon->resize(m_buttonSize+m_buttonMargin,
                                      m_buttonSize);
                m_currentIcon->setContentsMargins(m_contentMargin,
                                                  m_contentMargin,
                                                  m_contentMargin+m_buttonMargin,
                                                  m_contentMargin);
                //Ask to show the widget.
                emit requireShowWidget(widget);
            });
    //Change the caption's parent.
    caption->setParent(this);
    caption->hide();
    //Configure the widget.
    widget->hide();
    //Generate a tab and save the pointer.
    SidebarTab currentTab;
    currentTab.icon=icon;
    currentTab.caption=caption;
    currentTab.widget=widget;
    //Add the tab to list.
    m_tabList.append(currentTab);
    //Resize the widget.
    resize(m_foldIconSize,
           m_foldIconSize*m_tabList.size());
}

void KNSidebarContent::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Update the expand height.
    m_expandHeight=height()/3;
}

inline void KNSidebarContent::positionTab(const int &index,
                                          const SidebarTab &tab)
{
    //Make the parent of tab's content widgets to this.
    tab.widget->setParent(this);
    //Position the caption and content.
    tab.caption->move(0, m_expandHeight*index);
    tab.widget->move(0, m_expandHeight*index);
}

