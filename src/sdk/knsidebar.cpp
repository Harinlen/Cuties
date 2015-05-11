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

#include "knsidebar.h"

KNSidebar::KNSidebar(QWidget *parent) :
    QWidget(parent),
    m_mainMenu(new KNLabelAnimeButton(this))
{
    setObjectName("Sidebar");
    //Set properties.
    setAutoFillBackground(true);
    setPalette(KNGlobal::instance()->getPalette(objectName()));

    //Initial the main menu button.
    m_mainMenu->setObjectName("MainMenuButton");
    m_mainMenu->setPixmap(
                QPixmap(":/image/resource/images/icon.png").scaled(QSize(24,24),
                                                                   Qt::KeepAspectRatio,
                                                                   Qt::SmoothTransformation));
    m_mainMenu->setContentsMargins(3,3,3,3);
    m_mainMenu->setPalette(KNGlobal::instance()->getPalette(m_mainMenu->objectName()));
}
