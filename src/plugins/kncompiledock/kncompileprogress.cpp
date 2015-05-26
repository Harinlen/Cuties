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
#include <QProgressBar>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QTimeLine>

#include "knglobal.h"

#include "kncompileprogress.h"

#include <QDebug>

#define BAR_HEIGHT 72

KNCompileProgress::KNCompileProgress(QWidget *parent) :
    QWidget(parent),
    m_title(new QLabel(this)),
    m_progress(new QProgressBar(this)),
    m_showAnime(nullptr),
    m_hideAnime(nullptr)
{
    setObjectName("CompileProgress");
    //Set properties.
    setAutoFillBackground(true);
    setFixedSize(300, BAR_HEIGHT);
    //Set the palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
    //Get progress palette.
    m_progressPalette=KNGlobal::instance()->getPalette("CompileProgressBar");
    m_progress->setPalette(m_progressPalette);
    //Set layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    setLayout(mainLayout);

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_progress, 0, Qt::AlignVCenter);

    //Configure the progress bar.
    m_progress->setTextVisible(false);

    //Set shadow.
    QGraphicsDropShadowEffect *shadow=new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20.0);
    shadow->setOffset(0);
    shadow->setColor(QColor(0,0,0));
    setGraphicsEffect(shadow);

    //Configure anime.
    m_showAnime=generateAnime();
    m_hideAnime=generateAnime();
    connect(m_hideAnime, &QTimeLine::finished, this, &KNCompileProgress::hide);

    setTitle("Compiling");
    setProgress(50);
}

void KNCompileProgress::setTitle(const QString &caption)
{
    m_title->setText(caption);
}

void KNCompileProgress::setProgress(const int &progress)
{
    //If the progress is invaild, then set the progress.
    if(progress==-1)
    {
        //Fill all the progress bar to Text. (Like red)
        m_progress->setValue(m_progress->maximum());
        //Reset the palette.
        QPalette pal=m_progress->palette();
        pal.setColor(QPalette::Highlight, m_progressPalette.color(QPalette::Text));
        m_progress->setPalette(pal);
        return;
    }
    //If the progress is success.
    if(progress==100)
    {
        //Fill all the progress bar to AlternateBase. (Like green)
        m_progress->setValue(m_progress->maximum());
        //Reset the palette.
        QPalette pal=m_progress->palette();
        pal.setColor(QPalette::Highlight, m_progressPalette.color(QPalette::AlternateBase));
        m_progress->setPalette(pal);
        return;
    }
    //Or else, it should be the normal color.
    m_progress->setPalette(m_progressPalette);
    m_progress->setValue(progress);
}

void KNCompileProgress::onActionCompileProgressChange(const QString &caption,
                                                      int progress)
{
    //Set the title.
    setTitle(caption);
    //Set the progress.
    setProgress(progress);
}

void KNCompileProgress::showCompileProgress()
{
    //If the bar is on the showing way, then ignore the request.
    if(QTimeLine::Running==m_showAnime->state())
    {
        return;
    }
    //Stop the anime.
    m_hideAnime->stop();
    m_showAnime->stop();
    //Configure the moving anime.
    m_showAnime->setFrameRange(y(), 0);
    //Show the bar and start moving.
    show();
    m_showAnime->start();
}

void KNCompileProgress::hideCompileProgress()
{
    //If the bar is hiding, then ignore the request.
    if(QTimeLine::Running==m_hideAnime->state())
    {
        return;
    }
    //Stop the anime.
    m_hideAnime->stop();
    m_showAnime->stop();
    //Configure the moving anime.
    m_hideAnime->setFrameRange(y(), -BAR_HEIGHT);
    //Start moving.
    m_hideAnime->start();
}

void KNCompileProgress::changeYPosition(const int &positionY)
{
    move(x(), positionY);
}

QTimeLine *KNCompileProgress::generateAnime()
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setUpdateInterval(16);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNCompileProgress::changeYPosition);
    return timeLine;
}
