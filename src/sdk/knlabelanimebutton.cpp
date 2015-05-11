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

#include "knlabelanimebutton.h"

KNLabelAnimeButton::KNLabelAnimeButton(QWidget *parent) :
    QLabel(parent),
    m_backgroundAnime(new QTimeLine(200, this))
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
    //Start anime.
    startAnime(0);
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
    onActionUpdateBackground(100);
    QLabel::mousePressEvent(event);
}

void KNLabelAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Check flag.
    if(m_pressed)
    {
        emit clicked();
    }
    //Show released anime.
    startAnime(255);
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
