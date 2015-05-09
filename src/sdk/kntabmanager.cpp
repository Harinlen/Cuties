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
#include <QBoxLayout>
#include <QScrollArea>
#include <QSignalMapper>

#include "kntabmanageritem.h"
#include "kntabmanageritemselector.h"
#include "knglobal.h"

#include "kntabmanager.h"

#include <QDebug>

KNTabManager::KNTabManager(QWidget *parent) :
    QDockWidget(parent),
    m_container(new QWidget(this)),
    m_itemMapper(new QSignalMapper(this)),
    m_selector(new KNTabManagerItemSelector(this)),
    m_currentItem(nullptr)
{
    setObjectName("TabManager");
    //Set properties.
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFixedWidth(200);
    //Hack, make the title bar widget empty to disable the floating.
    setTitleBarWidget(new QWidget(this));

    //Set palette.
    QPalette managerPalette=KNGlobal::instance()->getPalette(objectName());
    setPalette(managerPalette);

    //Set the container scroll area.
    QScrollArea *scrollArea=new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidget(scrollArea);

    //Set container widget.
    m_container->setFixedWidth(200);
    m_container->setAutoFillBackground(true);
    m_container->setPalette(managerPalette);
    scrollArea->setWidget(m_container);
    m_selector->setParent(m_container);
    m_selector->move(200-m_selector->width(), 0);

    //Initialize the container layout.
    m_containerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                     m_container);
    m_containerLayout->setContentsMargins(0,0,0,0);
    m_containerLayout->setSpacing(0);
    m_container->setLayout(m_containerLayout);

    //Debug.
    for(int i=0; i<100; i++)
    {
        addTab("Label " + QString::number(i) + ".cpp");
    }
}

void KNTabManager::addTab(const QString &caption)
{
    //Generate the item.
    KNTabManagerItem *item=new KNTabManagerItem(caption, m_container);
    connect(item, &KNTabManagerItem::clicked,
            this, &KNTabManager::onActionItemClicked);
    //Add the item to list.
    m_itemList.append(item);
    //Add the item to layout, resize the container.
    m_containerLayout->addWidget(item);
    m_container->resize(m_container->sizeHint());
    //Raise the selector.
    m_selector->raise();
    //Check if this item is the first item, then set the current item to this
    //item.
    if(m_itemList.size()==1)
    {
        setCurrentIndex(0);
    }
}

void KNTabManager::setCurrentIndex(int index)
{
    Q_ASSERT(index>-1 && index<m_itemList.size());
    //Clear the original item.
    if(m_currentItem!=nullptr)
    {
        m_currentItem->setSelected(false);
    }
    //Get the item.
    m_currentItem=m_itemList.at(index);
    //Select the current item.
    m_currentItem->setSelected(true);
    //Move the selector.
    m_selector->move(m_selector->x(), m_currentItem->y());
}

void KNTabManager::setCurrentItem(KNTabManagerItem *item)
{
    ;
}

void KNTabManager::onActionItemClicked()
{
    //Ignore unavailable calling.
    if(sender()==nullptr)
    {
        return;
    }
    //Cast the sender as a item.
    KNTabManagerItem *clickedItem=qobject_cast<KNTabManagerItem *>(sender());
    //If we can find the item in the list, select the item.
    int itemIndex=m_itemList.indexOf(clickedItem);
    if(-1==itemIndex)
    {
        return;
    }
    setCurrentIndex(itemIndex);
}
