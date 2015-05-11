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
#include "knmainmenu.h"
#include "knsaosubmenu.h"
#include "knlabelanimebutton.h"

#include "knsidebar.h"

KNSidebar::KNSidebar(QWidget *parent) :
    QWidget(parent),
    m_mainMenuButton(new KNLabelAnimeButton(this)),
    m_mainMenu(new KNMainMenu(this))
{
    setObjectName("Sidebar");
    //Set properties.
    setAutoFillBackground(true);
    setMinimumWidth(30);
    setPalette(KNGlobal::instance()->getPalette(objectName()));
    resize(30, height());

    //Initial the main menu button.
    m_mainMenuButton->setObjectName("MainMenuButton");
    m_mainMenuButton->setPixmap(
                QPixmap(":/image/resource/images/icon.png").scaled(QSize(24,24),
                                                                   Qt::KeepAspectRatio,
                                                                   Qt::SmoothTransformation));
    m_mainMenuButton->setContentsMargins(3,3,3,3);
    QPalette mainMenuPalette=
            KNGlobal::instance()->getPalette(m_mainMenuButton->objectName());
    m_mainMenuButton->setPalette(mainMenuPalette);

    //Configure main menu.
    m_mainMenu->setObjectName("SAOSubMenu");
    //Initial all the sub menus.
    QString categoryIcons[MainMenuCategoryCount];
    categoryIcons[File]=":/icon/resource/icons/category/File.png";
    categoryIcons[Edit]=":/icon/resource/icons/category/Edit.png";
    categoryIcons[View]=":/icon/resource/icons/category/View.png";
    categoryIcons[Search]=":/icon/resource/icons/category/Search.png";
    categoryIcons[Execute]=":/icon/resource/icons/category/Run.png";
    categoryIcons[Debug]=":/icon/resource/icons/category/Debug.png";
    categoryIcons[Tools]=":/icon/resource/icons/category/Tools.png";
    categoryIcons[Tabs]=":/icon/resource/icons/category/Tabs.png";
    categoryIcons[Help]=":/icon/resource/icons/category/Help.png";
    for(int i=0; i<MainMenuCategoryCount; i++)
    {
        //Genereate the menu.
        m_categories[i]=new KNSAOSubMenu(m_mainMenu);
        m_categories[i]->menuAction()->setIcon(QIcon(categoryIcons[i]));
        m_mainMenu->addMenu(m_categories[i]);
    }
    //Link main menu button.
    connect(m_mainMenuButton, &KNLabelAnimeButton::clicked,
            this, &KNSidebar::onActionShowMainMenu);

    //Add exit command action to file menu.
    m_exitAction=new QAction(m_categories[File]);
    m_exitAction->setIcon(QIcon(":/icon/resource/icons/actions/exit.png"));
    m_categories[File]->addAction(m_exitAction);

    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNSidebar::retranslate);
    retranslate();

}

void KNSidebar::addCategoryAction(int category, QAction *action)
{
    //Ensure the category is available.
    if(category>-1 && category<MainMenuCategoryCount)
    {
        if(category==File)
        {
            //Add the action before exit action.
            m_categories[File]->insertAction(m_exitAction, action);
            return;
        }
        //Add action to the category.
        m_categories[category]->addAction(action);
    }
}

void KNSidebar::retranslate()
{
    //Update exit.
    m_exitAction->setText(tr("Exit"));
    //Update categories.
    m_categories[File]->setTitle(tr("File"));
    m_categories[Edit]->setTitle(tr("Edit"));
    m_categories[View]->setTitle(tr("View"));
    m_categories[Search]->setTitle(tr("Search"));
    m_categories[Execute]->setTitle(tr("Run"));
    m_categories[Debug]->setTitle(tr("Debug"));
    m_categories[Tools]->setTitle("Tools");
    m_categories[Tabs]->setTitle("Tabs");
    m_categories[Help]->setTitle("Help");
}

void KNSidebar::onActionShowMainMenu()
{
    QPoint buttonGlobalPos(mapToGlobal(m_mainMenuButton->pos()));
    m_mainMenu->exec(QPoint(buttonGlobalPos.x(),
                            buttonGlobalPos.y()+m_mainMenuButton->height()));
}