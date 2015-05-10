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
#include <QSignalMapper>
#include <QScrollBar>

#include "knsideshadowwidget.h"
#include "kntabmanageritem.h"
#include "kntabmanageritemselector.h"
#include "knglobal.h"

#include "kntabmanager.h"

#include <QDebug>

KNTabManager::KNTabManager(QWidget *parent) :
    QScrollArea(parent),
    m_container(new QWidget(this)),
    m_itemMapper(new QSignalMapper(this)),
    m_selector(new KNTabManagerItemSelector(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadow::TopShadow, this)),
    m_bottomShadow(new KNSideShadowWidget(KNSideShadow::BottomShadow, this)),
    m_currentItem(nullptr)
{
    setObjectName("TabManager");
    //Set properties.
    setFixedWidth(200);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Set palette.
    QPalette managerPalette=KNGlobal::instance()->getPalette(objectName());
    setPalette(managerPalette);

    //Set container widget.
    m_container->setFixedWidth(width());
    m_container->setAutoFillBackground(true);
    m_container->setPalette(managerPalette);
    m_container->lower();
    setWidget(m_container);
    m_selector->setParent(m_container);
    m_selector->move(200-m_selector->width(), 0);

    //Initialize the container layout.
    m_containerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                     m_container);
    m_containerLayout->setContentsMargins(0,0,0,0);
    m_containerLayout->setSpacing(0);
    m_container->setLayout(m_containerLayout);

    //Resize shadows
    m_topShadow->setGeometry(0,0,width(),15);
    m_topShadow->setBrightness(100);
    m_bottomShadow->setGeometry(0,102,width(),15);
    m_bottomShadow->setBrightness(100);
    //Link scrollbar signals.
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(onActionVerticalValueChanged(int)));

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

void KNTabManager::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QScrollArea::resizeEvent(event);
    //Move the shadow.
    m_bottomShadow->move(0, height()-m_bottomShadow->height());
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

void KNTabManager::onActionVerticalValueChanged(const int &value)
{
    //When the vertical scroll bar reaches the minimum, hide the top shadow.
    m_topShadow->setVisible(verticalScrollBar()->minimum()!=value);
    //When the vertival scroll bar reaches the maximum, hide the bottom shadow.
    m_bottomShadow->setVisible(verticalScrollBar()->maximum()!=value);
}
