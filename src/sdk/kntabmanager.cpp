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
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "knsidebar.h"
#include "knsideshadowwidget.h"
#include "kntextedit.h"
#include "kncodeeditor.h"
#include "kncodeeditorunibar.h"
#include "kntabmanageritem.h"
#include "kntabmanagercontent.h"
#include "kncompiledockbase.h"
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
    m_compileDock(nullptr),
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

    //Configure content.
    m_content->setMinimumHeight(81);

    //Initial the actions.
    initialActions();
    //Link retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNTabManager::retranslate);
    retranslate();
}

void KNTabManager::setSidebar(KNSidebar *sidebar)
{
    //Get the main window widget.
    QWidget *window=sidebar->parentWidget();
    for(int i=0; i<TabManagerActionCount; i++)
    {
        window->addAction(m_actions[i]);
    }
    //Add actions to main menu.
    sidebar->addCategoryAction(File, m_actions[New]);
    sidebar->addCategoryAction(File, m_actions[Open]);
    sidebar->addCategoryAction(File, m_actions[Save]);
    sidebar->addCategoryAction(File, m_actions[SaveAs]);
    sidebar->addCategoryAction(File, m_actions[SaveAll]);
    sidebar->addCategoryAction(File, m_actions[Close]);
    sidebar->addCategoryAction(File, m_actions[CloseAll]);
    sidebar->addCategoryAction(File, m_actions[CloseAllOthers]);

    //Add actions to build menu.
    sidebar->addCategoryAction(Build, m_actions[Compile]);
}

QWidget *KNTabManager::contentWidget()
{
    return m_content;
}

KNTabManagerItem *KNTabManager::createTab(const QString &caption,
                                          const QString &filePath)
{
    //Generate the item.
    KNTabManagerItem *item=new KNTabManagerItem(caption, m_container);
    //Link the item and add to item list.
    connect(item, &KNTabManagerItem::clicked,
            this, &KNTabManager::onActionItemClicked);
    connect(item, &KNTabManagerItem::requireCloseTab,
            this, &KNTabManager::closeTab);
    //Get the code editor.
    KNCodeEditor *codeEditor=item->codeEditor();
    m_content->addWidget(codeEditor);
    //If there's a file to load, load the file.
    if(!filePath.isEmpty())
    {
        //Get the file information.
        QFileInfo fileInfo(filePath);
        //Reset the item name.
        item->setCaption(fileInfo.fileName());
        //Configure the code editor of the item.
        codeEditor->setLanguageMode(fileInfo.suffix());
        //Open the file and set the file path.
        codeEditor->openFile(filePath);
        codeEditor->setFilePath(filePath);
    }
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
    //Return the item.
    return item;
}

void KNTabManager::setCurrentIndex(int index)
{
    Q_ASSERT(index>-1 && index<m_itemList.size());
    //Check we need to set the item or not.
    if(m_itemList.at(index)==m_currentItem)
    {
        return;
    }
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
    m_currentItem->codeEditor()->textEditor()->setFocus();
    //Connect the unibar to the editor.
    if(m_unibar!=nullptr)
    {
        m_unibar->setEditor(m_currentItem->codeEditor());
    }
    //Connect the compile bar to the editor.
    if(m_compileDock!=nullptr)
    {
        m_compileDock->setCodeEditor(m_currentItem->codeEditor());
    }
}

void KNTabManager::setCurrentItem(KNTabManagerItem *item)
{
    //Check we need to set the item at the beginning.
    if(item==m_currentItem)
    {
        return;
    }
    //If the item is nullptr, then clear the current item and reset the unibar.
    if(item==nullptr)
    {
        //Clear current item.
        m_currentItem=nullptr;
        if(m_unibar!=nullptr)
        {
            m_unibar->setEditor(nullptr);
        }
        if(m_compileDock!=nullptr)
        {
            m_compileDock->setCodeEditor(nullptr);
        }
        return;
    }
    //Get the index of the current item.
    setCurrentIndex(m_itemList.indexOf(item));
}

bool KNTabManager::closeTab(KNTabManagerItem *item)
{
    //Check item is null or not.
    if(item==nullptr)
    {
        return false;
    }
    KNTabManagerItem *nextCurrentItem=m_currentItem;
    if(item==m_currentItem)
    {
        //Get the current item index.
        int itemIndex=m_itemList.indexOf(item);
        //Check the current index and the item size.
        if((itemIndex+1)<m_itemList.size())
        {
            nextCurrentItem=m_itemList.at(itemIndex+1);
        }
        else
        {
            //There's still previous item of the current index
            if((itemIndex-1)>-1)
            {
                nextCurrentItem=m_itemList.at(itemIndex-1);
            }
            else
            {
                nextCurrentItem=nullptr;
            }
        }
    }
    //Remove the item, if success, set current item to the nextCurrentItem.
    if(removeItem(item))
    {
        setCurrentItem(nextCurrentItem);
    }
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
    m_actions[Compile]->setText(tr("Compile"));

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
    setCurrentItem(createTab(m_untitledPrefix +
                             " " +
                             QString::number(m_untitledCounter)));
}

void KNTabManager::onActionOpen()
{
    //Get the file path.
    QStringList filePathList=QFileDialog::getOpenFileNames(this,
                                                           tr("Open"));
    if(filePathList.isEmpty())
    {
        return;
    }
    //Open all of these files.
    QString currentPath;
    KNTabManagerItem *lastItem;
    while(!filePathList.isEmpty())
    {
        currentPath=filePathList.takeFirst();
        //Generate the item.
        lastItem=createTab("", currentPath);
    }
    //Set the current item to the last opened item.
    setCurrentItem(lastItem);
}

void KNTabManager::onActionSave()
{
    saveItem(m_currentItem);
}

void KNTabManager::onActionSaveAs()
{
    saveAsItem(m_currentItem);
}

void KNTabManager::onActionSaveAll()
{
    //Save all the item.
    for(QList<KNTabManagerItem *>::iterator i=m_itemList.begin();
        i!=m_itemList.end();
        ++i)
    {
        saveItem(*i);
    }
}

void KNTabManager::onActionClose()
{
    //Close the current item.
    if(m_currentItem!=nullptr)
    {
        closeTab(m_currentItem);
    }
}

void KNTabManager::onActionCloseAll()
{
    //Set the current item to nullptr.
    setCurrentItem(nullptr);
    //Remove all the item.
    while(!m_itemList.isEmpty())
    {
        if(!removeItem(m_itemList.takeLast()))
        {
            break;
        }
    }
    //Resize the container.
    m_container->setFixedHeight(KNTabManagerItem::itemHeight() * m_itemList.size());
}

void KNTabManager::onActionCloseAllOthers()
{
    //Close all the item, then insert the current item to item list.
    while(!m_itemList.isEmpty())
    {
        //Close the other items.
        KNTabManagerItem *item=m_itemList.takeLast();
        if(item!=m_currentItem)
        {
            if(!removeItem(item))
            {
                break;
            }
        }
    }
    //Restore the current item to the item list.
    if(!m_itemList.contains(m_currentItem))
    {
        //Insert the missing current item.
        m_itemList.append(m_currentItem);
    }
    //Resize the container.
    m_container->setFixedHeight(KNTabManagerItem::itemHeight() * m_itemList.size());
}

void KNTabManager::onActionCompile()
{
    //Check current item, ignore the null item.
    if(m_currentItem==nullptr)
    {
        return;
    }
    //Save the current item first.
    if(saveItem(m_currentItem))
    {
        //Compile the current item.
        m_currentItem->codeEditor()->compile();
    }
}

inline bool KNTabManager::saveItem(KNTabManagerItem *item)
{
    //Check the current item.
    if(item==nullptr)
    {
        return false;
    }
    //Get the code editor.
    KNCodeEditor *codeEditor=item->codeEditor();
    //Check if the file path can save or not.
    if(codeEditor->filePath().isEmpty())
    {
        return saveAsItem(item);
    }
    //Save the file.
    return codeEditor->saveFile();
}

inline bool KNTabManager::saveAsItem(KNTabManagerItem *item)
{
    //Get the file path.
    QString filePath=QFileDialog::getSaveFileName(this,
                                                  tr("Save"));
    if(filePath.isEmpty())
    {
        return false;
    }
    //Get the code editor.
    KNCodeEditor *codeEditor=item->codeEditor();
    //Set the file path.
    codeEditor->setFilePath(filePath);
    //Save the file.
    return codeEditor->saveFile();
}

bool KNTabManager::removeItem(KNTabManagerItem *item)
{
    //Ignore the illegal request.
    if(item==nullptr)
    {
        return false;
    }
    //Get the code editor.
    KNCodeEditor *editor=item->codeEditor();
    //We need to check the editor is need to be save.
    if(editor->isModified())
    {
        //There are two states:
        //1. This file is a new created file.
        if((editor->filePath().isEmpty() && (!editor->isEmpty())) ||
                !editor->filePath().isEmpty())
        {
            //We need to ask whether we need to save the file.
            int result=QMessageBox::question(this,
                                             tr("Save modified document"),
                                             tr("Do you want to save the changes you made in the document \"%1\"?").arg(item->text()),
                                             tr("Save"),
                                             tr("Cancel"),
                                             tr("Don't Save"),
                                             0,
                                             1);
            switch(result)
            {
            case 0: //Save
            {
                //We need to save the document.
                if(!saveItem(item))
                {
                    return false;
                }
                break;
            }
            case 1: //Cancel
            {
                //Just return false.
                return false;
            }
            case 2:  //Don't save.
            {
                //Go on to destory the item.
                break;
            }
            }
        }
    }
    //Remove the editor from the stacked widget.
    m_content->removeWidget(editor);
    //Give the parent of the code editor back to item.
    editor->setParent(item);
    //Remove the item from the item list.
    //Although use remove one is not the perfect choice, but there can't be two
    //same item in the list, so use removeOne is not wrong.
    m_itemList.removeOne(item);
    //Resize the container.
    m_container->setFixedHeight(KNTabManagerItem::itemHeight()*m_itemList.size());
    //Delete the item.
    item->deleteLater();
    return true;
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
    actionIcon[Compile]=":/icon/resource/icons/actions/compile.png";

    for(int i=0; i<TabManagerActionCount; i++)
    {
        m_actions[i]=new QAction(this);
        m_actions[i]->setIcon(QIcon(actionIcon[i]));
        m_actions[i]->setShortcutContext(Qt::ApplicationShortcut);
    }

    //Set key sequences.
    m_actions[New]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    m_actions[Open]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_actions[Save]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    m_actions[SaveAs]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    m_actions[SaveAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_S));
    m_actions[Close]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_W));
    m_actions[CloseAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_W));
    m_actions[CloseAllOthers]->setShortcut(QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_W));

    //Link the action with the slots.
    connect(m_actions[New], SIGNAL(triggered()),
            this, SLOT(onActionNewSourceFile()));
    connect(m_actions[Open], SIGNAL(triggered()),
            this, SLOT(onActionOpen()));
    connect(m_actions[Save], SIGNAL(triggered()),
            this, SLOT(onActionSave()));
    connect(m_actions[SaveAs], SIGNAL(triggered()),
            this, SLOT(onActionSaveAs()));
    connect(m_actions[SaveAll], SIGNAL(triggered()),
            this, SLOT(onActionSaveAll()));
    connect(m_actions[Close], SIGNAL(triggered()),
            this, SLOT(onActionClose()));
    connect(m_actions[CloseAll], SIGNAL(triggered()),
            this, SLOT(onActionCloseAll()));
    connect(m_actions[CloseAllOthers], SIGNAL(triggered()),
            this, SLOT(onActionCloseAllOthers()));

    connect(m_actions[Compile], SIGNAL(triggered()),
            this, SLOT(onActionCompile()));
}

KNCompileDockBase *KNTabManager::compileDock() const
{
    return m_compileDock;
}

void KNTabManager::setCompileDock(KNCompileDockBase *compileDock)
{
    //Save the compile dock pointer.
    m_compileDock = compileDock;
    //Link the compile dock.
    connect(m_compileDock, &KNCompileDockBase::requireGoto,
            [=](const int &line, const int &column)
            {
                //Check current item first.
                if(m_currentItem==nullptr)
                {
                    return;
                }
                //Use the set position function to goto the right position.
                m_currentItem->codeEditor()->setTextCursorPosition(line, column);
            });
}

KNCodeEditorUnibar *KNTabManager::unibar() const
{
    return m_unibar;
}

void KNTabManager::setUnibar(KNCodeEditorUnibar *unibar)
{
    //Save unibar.
    m_unibar=unibar;
    //Link the unibar.
    connect(m_unibar, &KNCodeEditorUnibar::requireCloseCurrent,
            m_actions[Close], &QAction::trigger);
}

