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

#include "kntexteditpanel.h"

#include "kntexteditpanelcontainer.h"

#include <QDebug>

KNTextEditPanelContainer::KNTextEditPanelContainer(QWidget *parent) :
    QWidget(parent),
    m_panelLayout(new QBoxLayout(QBoxLayout::LeftToRight, this)),
    m_editor(nullptr)
{
    //Configure and set layout.
    m_panelLayout->setContentsMargins(0,0,0,0);
    m_panelLayout->setSpacing(0);
    setLayout(m_panelLayout);
}

void KNTextEditPanelContainer::addPanel(KNTextEditPanel *panel)
{
    //Configure the panel.
    panel->setEditor(m_editor);
    m_panelList.append(panel);
    connect(panel, &KNTextEditPanel::requireResizeMargin,
            this, &KNTextEditPanelContainer::requireResizeMargin);
    //Add panel to layout.
    m_panelLayout->addWidget(panel);
}

int KNTextEditPanelContainer::panelWidth()
{
    int panelWidth=0;
    //Update all the panels.
    for(QLinkedList<KNTextEditPanel *>::iterator i=m_panelList.begin();
        i!=m_panelList.end();
        ++i)
    {
        panelWidth+=(*i)->width();
    }
    return panelWidth;
}

void KNTextEditPanelContainer::setEditor(KNTextEdit *editor)
{
    m_editor=editor;
}

void KNTextEditPanelContainer::updatePanels()
{
    //Update all the panels.
    for(QLinkedList<KNTextEditPanel *>::iterator i=m_panelList.begin();
        i!=m_panelList.end();
        ++i)
    {
        (*i)->update();
    }
}

void KNTextEditPanelContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the container.
    QWidget::resizeEvent(event);
    //Emit resize margin signal.
    emit requireResizeMargin();
}
