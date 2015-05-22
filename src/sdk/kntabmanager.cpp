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
#include <QAction>
#include <QBoxLayout>
#include <QSignalMapper>
#include <QScrollBar>

#include "knsidebar.h"
#include "knsideshadowwidget.h"
#include "kncodeeditor.h"
#include "kncodeeditorunibar.h"
#include "kntabmanageritem.h"
#include "kntabmanagercontent.h"
#include "knglobal.h"

#include "kntabmanager.h"

#include <QDebug>

KNTabManager::KNTabManager(QWidget *parent) :
    QScrollArea(parent),
    m_unibar(nullptr),
    m_container(new QWidget(this)),
    m_itemMapper(new QSignalMapper(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadow::TopShadow, this)),
    m_bottomShadow(new KNSideShadowWidget(KNSideShadow::BottomShadow, this)),
    m_content(new KNTabManagerContent(this)),
    m_currentItem(nullptr),
    m_untitledCounter(0)
{
    setObjectName("TabManager");
    //Set properties.
    setMinimumWidth(200);
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

    //Initialize the container layout.
    m_containerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                     m_container);
    m_containerLayout->setContentsMargins(0,0,0,0);
    m_containerLayout->setSpacing(0);
    m_container->setLayout(m_containerLayout);

    //Resize shadows
    m_topShadow->hide();
    m_topShadow->setGeometry(0,0,width(),15);
    m_topShadow->setBrightness(100);
    m_bottomShadow->hide();
    m_bottomShadow->setGeometry(0,102,width(),15);
    m_bottomShadow->setBrightness(100);
    //Link scrollbar signals.
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNTabManager::onActionVerticalValueChanged);

    //Initial the actions.
    initialActions();
    //Link retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNTabManager::retranslate);
    retranslate();
}

void KNTabManager::setSidebar(KNSidebar *sidebar)
{
    //Add actions to main menu.
    sidebar->addCategoryAction(File, m_actions[New]);
    sidebar->addCategoryAction(File, m_actions[Open]);
    sidebar->addCategoryAction(File, m_actions[Save]);
    sidebar->addCategoryAction(File, m_actions[SaveAs]);
    sidebar->addCategoryAction(File, m_actions[SaveAll]);
    sidebar->addCategoryAction(File, m_actions[Close]);
    sidebar->addCategoryAction(File, m_actions[CloseAll]);
    sidebar->addCategoryAction(File, m_actions[CloseAllOthers]);
}

QWidget *KNTabManager::contentWidget()
{
    return m_content;
}

void KNTabManager::createTab(const QString &caption, const QString &suffix)
{
    //Generate the item.
    KNTabManagerItem *item=new KNTabManagerItem(caption, m_container);
    //Link the item and add to item list.
    connect(item, &KNTabManagerItem::clicked,
            this, &KNTabManager::onActionItemClicked);
    //Configure the code editor of the item.
    KNCodeEditor *codeEditor=item->codeEditor();
    codeEditor->setLanguageMode(suffix);
    m_content->addWidget(codeEditor);
    //Add the item to list.
    m_itemList.append(item);
    //Add the item to layout, resize the container.
    m_containerLayout->addWidget(item);
    m_container->setFixedHeight(KNTabManagerItem::itemHeight()*m_itemList.size());
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
    //Switch the content to the item.
    m_content->setCurrentIndex(index);
    //Set focus to the content widget.
    //Check if current widget is nullptr.
    QWidget *currentWidget=m_content->currentWidget();
    currentWidget->setFocus(Qt::MouseFocusReason);
    //Connect the unibar to the editor.
    if(m_unibar!=nullptr)
    {
        m_unibar->setEditor(m_currentItem->codeEditor());
    }
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
    //Resize the content.
    m_container->setFixedWidth(width());
}

void KNTabManager::retranslate()
{
    //Translate actions.
    m_actions[New]->setText(tr("New Source File"));
    m_actions[Open]->setText(tr("Open"));
    m_actions[Save]->setText(tr("Save"));
    m_actions[SaveAs]->setText(tr("Save As"));
    m_actions[SaveAll]->setText(tr("Save All"));
    m_actions[Close]->setText(tr("Close"));
    m_actions[CloseAll]->setText(tr("Close All"));
    m_actions[CloseAllOthers]->setText(tr("Close All Other Files"));

    //Translate title.
    m_untitledPrefix=tr("Untitled");
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

void KNTabManager::onActionNewSourceFile()
{
    //Add counter, generate the counter.
    m_untitledCounter++;
    //Create a new tab.
    createTab(m_untitledPrefix + " " + QString::number(m_untitledCounter));
}

void KNTabManager::initialActions()
{
    QString actionIcon[TabManagerActionCount];
    actionIcon[New]=":/icon/resource/icons/actions/new.png";
    actionIcon[Open]=":/icon/resource/icons/actions/open.png";
    actionIcon[Save]=":/icon/resource/icons/actions/save.png";
    actionIcon[SaveAs]=":/icon/resource/icons/actions/saveas.png";
    actionIcon[SaveAll]=":/icon/resource/icons/actions/saveall.png";
    actionIcon[Close]=":/icon/resource/icons/actions/close.png";
    actionIcon[CloseAll]=":/icon/resource/icons/actions/close.png";
    actionIcon[CloseAllOthers]=":/icon/resource/icons/actions/close.png";

    for(int i=0; i<TabManagerActionCount; i++)
    {
        m_actions[i]=new QAction(this);
        m_actions[i]->setIcon(QIcon(actionIcon[i]));
    }

    //Link the action with the slots.
    connect(m_actions[New], SIGNAL(triggered()),
            this, SLOT(onActionNewSourceFile()));
}

KNCodeEditorUnibar *KNTabManager::unibar() const
{
    return m_unibar;
}

void KNTabManager::setUnibar(KNCodeEditorUnibar *unibar)
{
    m_unibar = unibar;
}

