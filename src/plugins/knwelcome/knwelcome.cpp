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
#include <QLabel>
#include <QBoxLayout>

#include "knversion.h"
#include "knglobal.h"
#include "knsideshadowwidget.h"

#include "knwelcome.h"

#include <QDebug>

KNWelcome::KNWelcome(QWidget *parent) :
    KNWelcomeBase(parent),
    m_banner(new QLabel(this))
{
    setObjectName("Welcome");
    //Set properties.
    setAutoFillBackground(true);

    //Set palette.
    QPalette pal=KNGlobal::instance()->getPalette(objectName());
    setPalette(pal);

    //Configure the label.
    QPixmap bannerImage(":/image/resource/images/welcome_banner.png");
    m_banner->setPixmap(bannerImage);
    //Set the fixed size of the welcome window.
    setFixedSize(bannerImage.width(), 450);

    //Generate the layout for the banner.
    QBoxLayout *bannerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_banner);
    m_banner->setLayout(bannerLayout);
    QLabel *appIcon=new QLabel(this);
    appIcon->setPixmap(QPixmap(""));
    bannerLayout->addWidget(appIcon);
    QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             bannerLayout->widget());
    bannerLayout->addLayout(captionLayout, 1);

    //Add caption label.
    captionLayout->addStretch();
    QLabel *caption=new QLabel("Kreogist Dev Team");
    caption->setPalette(pal);
    QFont captionFont=font();
    captionFont.setPixelSize(15);
    captionFont.setWeight(0);
    caption->setFont(captionFont);
    captionLayout->addWidget(caption);
    caption=new QLabel("Cuties " + QString::number(MAJOR_VERSION));
    caption->setPalette(pal);
    captionFont.setPixelSize(30);
    captionFont.setWeight(50);
    caption->setFont(captionFont);
    captionLayout->addWidget(caption);
    captionLayout->addStretch();

    //Initial the shadow for the side shadow.
    KNSideShadowWidget *topShadow=new KNSideShadowWidget(TopShadow, this);
    topShadow->move(0, bannerImage.height());
    topShadow->resize(bannerImage.width(), 13);
}
