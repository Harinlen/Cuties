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
#include <QTimeLine>

#include "knglobal.h"

#include "knwelcomenewbutton.h"

KNWelcomeNewButton::KNWelcomeNewButton(QWidget *parent) :
    QLabel(parent),
    m_pressed(false),
    m_mouseInOutAnime(new QTimeLine(200, this)),
    m_itemIcon(new QLabel(this))
{
    setObjectName("WelcomeNewButton");
    //Save the palette.
    QPalette pal=KNGlobal::instance()->getPalette(objectName());
    QColor windowColor=pal.color(QPalette::Window);
    windowColor.setAlpha(0);
    pal.setColor(QPalette::Window, windowColor);
    pal.setColor(QPalette::WindowText, QColor(0,0,0));
    setPalette(pal);
    setAutoFillBackground(true);
    //Configure the anime.
    m_mouseInOutAnime->setUpdateInterval(15);
    m_mouseInOutAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseInOutAnime, &QTimeLine::frameChanged,
            this, &KNWelcomeNewButton::onActionInOutAnime);
    //Configure the button.
    setFixedHeight(32);
    setContentsMargins(32,0,32,0);
    //Configure the item icon.
    m_itemIcon->setScaledContents(true);
    m_itemIcon->setFixedSize(32, 32);
}

void KNWelcomeNewButton::setItemIcon(const QPixmap &icon)
{
    m_itemIcon->setPixmap(icon);
}

void KNWelcomeNewButton::enterEvent(QEvent *event)
{
    //Do enter event first.
    QLabel::enterEvent(event);
    //Start enter anime.
    startMouseInOutAnime(255);
}

void KNWelcomeNewButton::leaveEvent(QEvent *event)
{
    //Do enter event first.
    QLabel::leaveEvent(event);
    //Start leave anime.
    startMouseInOutAnime(0);
}

void KNWelcomeNewButton::mousePressEvent(QMouseEvent *event)
{
    m_pressed=true;
    QLabel::mousePressEvent(event);
}

void KNWelcomeNewButton::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
}

void KNWelcomeNewButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_pressed)
    {
        m_pressed=false;
        emit clicked();
    }
    QLabel::mouseReleaseEvent(event);
}

void KNWelcomeNewButton::onActionInOutAnime(int frame)
{
    //Make the frame always available.
    if(frame<0)
    {
        frame=0;
    }
    if(frame>255)
    {
        frame=255;
    }
    //Change the color's transparency.
    QPalette pal=palette();
    QColor windowColor=pal.color(QPalette::Window);
    windowColor.setAlpha(frame);
    pal.setColor(QPalette::Window, windowColor);
    pal.setColor(QPalette::WindowText, QColor(frame, frame, frame));
    setPalette(pal);
}

void KNWelcomeNewButton::startMouseInOutAnime(const int &targetFrame)
{
    //Stop the anime first.
    m_mouseInOutAnime->stop();
    //Set the frame range.
    m_mouseInOutAnime->setFrameRange(palette().color(QPalette::Window).alpha(),
                                     targetFrame);
    //Start the anime.
    m_mouseInOutAnime->start();
}

QString KNWelcomeNewButton::suffix() const
{
    return m_suffix;
}

void KNWelcomeNewButton::setSuffix(const QString &suffix)
{
    m_suffix = suffix;
}
