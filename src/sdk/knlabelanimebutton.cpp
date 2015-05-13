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
#include <QMouseEvent>
#include <QAction>
#include <QTimeLine>

#include "knlabelanimebutton.h"

KNLabelAnimeButton::KNLabelAnimeButton(QWidget *parent) :
    QLabel(parent),
    m_backgroundAnime(new QTimeLine(200, this)),
    m_pressed(false),
    m_checkable(false),
    m_checked(false)
{
    //Set properties.
    setAutoFillBackground(true);
    //Configure time line.
    m_backgroundAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_backgroundAnime->setUpdateInterval(16);
    connect(m_backgroundAnime, &QTimeLine::frameChanged,
            this, &KNLabelAnimeButton::onActionUpdateBackground);
}

void KNLabelAnimeButton::setPalette(const QPalette &pal)
{
    //Set the palette.
    QLabel::setPalette(pal);
    //Initial the palette.
    onActionUpdateBackground(0);
}

void KNLabelAnimeButton::enterEvent(QEvent *event)
{
    //Start anime.
    startAnime(255);
    //Emit entered signal.
    emit entered();
    //Do enter event.
    QLabel::enterEvent(event);
}

void KNLabelAnimeButton::leaveEvent(QEvent *event)
{
    //If the button is not checked.
    if(!(m_checkable && m_checked))
    {
        //Start anime.
        startAnime(0);
    }
    //Emit leaved signal.
    emit leaved();
    //Do leave event.
    QLabel::leaveEvent(event);
}

void KNLabelAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Set pressed flag.
    m_pressed=true;
    //Show pressed background.
    onActionUpdateBackground(200);
    QLabel::mousePressEvent(event);
}

void KNLabelAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Check flag.
    if(m_pressed)
    {
        //Check the event.
        if(event->button()==Qt::LeftButton)
        {
            //If the button is checkable, set the button to check mode.
            if(m_checkable)
            {
                setChecked(!m_checked);
            }
            emit clicked();
        }
        else if(event->button()==Qt::RightButton)
        {
            emit rightClicked();
        }
    }
    //Show released anime.
    if(!m_checkable)
    {
        startAnime(255);
    }
    QLabel::mouseReleaseEvent(event);
}

void KNLabelAnimeButton::onActionUpdateBackground(const int &frame)
{
    //Change the background lightness.
    QPalette pal=palette();
    QColor backgroundColor=pal.color(QPalette::Window);
    backgroundColor.setAlpha(frame);
    pal.setColor(QPalette::Window, backgroundColor);
    QLabel::setPalette(pal);
}

void KNLabelAnimeButton::startAnime(const int &end)
{
    //Stop the anime.
    m_backgroundAnime->stop();
    //Set range.
    m_backgroundAnime->setFrameRange(palette().color(QPalette::Window).alpha(),
                                     end);
    //Start the anime.
    m_backgroundAnime->start();
}
bool KNLabelAnimeButton::checked() const
{
    return m_checked;
}

void KNLabelAnimeButton::setChecked(bool checked)
{
    if(m_checkable)
    {
        m_checked = checked;
        //Change the background according to the state.
        startAnime(m_checked?255:0);
    }
}

bool KNLabelAnimeButton::checkable() const
{
    return m_checkable;
}

void KNLabelAnimeButton::setCheckable(bool checkable)
{
    m_checkable = checkable;
}

