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
#include "knwelcomebase.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_welcome(nullptr)
{
    setObjectName("MainWindow");
    //Set properties.
    setMinimumSize(950, 600);
}

KNWelcomeBase *KNMainWindow::welcome() const
{
    return m_welcome;
}

void KNMainWindow::setWelcome(KNWelcomeBase *welcome)
{
    //Save the welcome pointer.
    m_welcome = welcome;
    //Check the welcome is null or not.
    if(m_welcome==nullptr)
    {
        return;
    }
    //Configure the welcome widget.
    m_welcome->setParent(this);
}

void KNMainWindow::resizeEvent(QResizeEvent *event)
{
    //Resize the main window.
    QMainWindow::resizeEvent(event);
    //Check the welcome is visible or not.
    if(m_welcome && m_welcome->isVisible())
    {
        m_welcome->move((width()-m_welcome->width())>>1,
                        (height()-m_welcome->height())>>1);
    }
}
