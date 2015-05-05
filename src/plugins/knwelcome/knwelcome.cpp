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
#include <QListView>
#include <QSignalMapper>

#include "knversion.h"
#include "knglobal.h"
#include "knwelcomenewbutton.h"
#include "knsideshadowwidget.h"

#include "knwelcome.h"

#include <QDebug>

KNWelcome::KNWelcome(QWidget *parent) :
    KNWelcomeBase(parent),
    m_banner(new QLabel(this)),
    m_newCaption(new QLabel(this)),
    m_openCaption(new QLabel(this)),
    m_newButtonMapper(new QSignalMapper(this))
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
    m_banner->setFixedSize(bannerImage.size());
    //Set the fixed size of the welcome window.
    setFixedSize(bannerImage.width(), 450);

    //Link the button mapper to the signal.
    connect(m_newButtonMapper, SIGNAL(mapped(QString)),
            this, SIGNAL(requireNewFile(QString)));

    //Generate the layout for the banner.
    QBoxLayout *bannerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_banner);
    bannerLayout->setSpacing(15);
    m_banner->setLayout(bannerLayout);
    QLabel *appIcon=new QLabel(this);
    appIcon->setFixedSize(85, 85);
    appIcon->setScaledContents(true);
    appIcon->setPixmap(QPixmap(":/image/resource/images/icon.png"));
    bannerLayout->addWidget(appIcon);
    QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             bannerLayout->widget());
    captionLayout->setSpacing(0);
    bannerLayout->addLayout(captionLayout, 1);

    QFont captionFont=font();
    //Add caption label.
    captionLayout->addStretch();

    QLabel *caption2=new QLabel("Cuties " + QString::number(MAJOR_VERSION));
    caption2->setPalette(pal);
    captionFont.setPixelSize(35);
    captionFont.setWeight(50);
    caption2->setFont(captionFont);
    captionLayout->addWidget(caption2);

    QLabel *caption=new QLabel("(C) Kreogist Dev Team. All rights reserved.");
    caption->setPalette(pal);
    captionFont.setPixelSize(10);
    captionFont.setWeight(0);
    caption->setFont(captionFont);
    captionLayout->addWidget(caption);

    captionLayout->addStretch();

    //Initial the shadow for the side shadow.
    KNSideShadowWidget *topShadow=new KNSideShadowWidget(TopShadow, this);
    topShadow->move(0, bannerImage.height());
    topShadow->resize(bannerImage.width(), 10);

    //Initial the body layout.
    QBoxLayout *bodyLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    bodyLayout->setContentsMargins(0,0,0,0);
    setLayout(bodyLayout);
    bodyLayout->addSpacing(bannerImage.height()+topShadow->height());

    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             bodyLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    bodyLayout->addLayout(contentLayout, 1);

    QBoxLayout *createLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            bodyLayout->widget());
    createLayout->setContentsMargins(0,0,0,0);
    contentLayout->addLayout(createLayout);

    QBoxLayout *openLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          bodyLayout->widget());
    contentLayout->addLayout(openLayout);

    //Add widgets to 'new' layout.
    createLayout->addWidget(m_newCaption);
    m_newLayout=new QBoxLayout(QBoxLayout::TopToBottom, createLayout->widget());
    m_newLayout->setSpacing(0);
    createLayout->addLayout(m_newLayout);
    createLayout->addStretch();

    //Add default new suffix.
    KNWelcomeNewButton *createEmptySuffix=new KNWelcomeNewButton(this);
    createEmptySuffix->setSuffix("");
    createEmptySuffix->setItemIcon(QPixmap(":/icon/resource/icons/file/PlainText/PlainText_64x64.png"));
    addNewButton(createEmptySuffix);

    createEmptySuffix=new KNWelcomeNewButton(this);
    createEmptySuffix->setSuffix("");
    createEmptySuffix->setItemIcon(QPixmap(":/icon/resource/icons/file/PlainText/PlainText_64x64.png"));
    addNewButton(createEmptySuffix);

    createEmptySuffix=new KNWelcomeNewButton(this);
    createEmptySuffix->setSuffix("");
    createEmptySuffix->setItemIcon(QPixmap(":/icon/resource/icons/file/PlainText/PlainText_64x64.png"));
    addNewButton(createEmptySuffix);

    createEmptySuffix=new KNWelcomeNewButton(this);
    createEmptySuffix->setSuffix("");
    createEmptySuffix->setItemIcon(QPixmap(":/icon/resource/icons/file/PlainText/PlainText_64x64.png"));
    addNewButton(createEmptySuffix);

    //Add widgets to 'open' layout.
    openLayout->addWidget(m_openCaption);
    QListView *recentList=new QListView(this);
    openLayout->addWidget(recentList);

    //Link the retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNWelcome::retranslate);
    retranslate();
}

void KNWelcome::retranslate()
{
    //Update captions.
    m_newCaption->setText(tr("New Source File"));
    m_openCaption->setText(tr("Open Source File"));

    //Update new buttons.
    QLinkedList<KNWelcomeNewButton *>::iterator i=m_newButtonList.begin();
    (*i)->setText(tr("New Plain Text"));
}

inline void KNWelcome::addNewButton(KNWelcomeNewButton *button)
{
    //Link the button to the button mapper.
    connect(button, SIGNAL(clicked()), m_newButtonMapper, SLOT(map()));
    m_newButtonMapper->setMapping(button, button->suffix());
    //Add button to button list.
    m_newButtonList.append(button);
    //Add button to layout.
    m_newLayout->addWidget(button);
}
