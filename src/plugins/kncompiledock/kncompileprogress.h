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

#ifndef KNCOMPILEPROGRESS_H
#define KNCOMPILEPROGRESS_H

#include <QWidget>

class QLabel;
class QProgressBar;
class QTimeLine;
class KNCompileProgress : public QWidget
{
    Q_OBJECT
public:
    explicit KNCompileProgress(QWidget *parent = 0);
    void setTitle(const QString &caption);
    void setProgress(const int &progress);

signals:

public slots:
    void onActionCompileProgressChange(const QString &caption,
                                       int progress);
    void showCompileProgress();
    void hideCompileProgress();

private slots:
    void changeYPosition(const int &positionY);

private:
    inline QTimeLine *generateAnime();
    QLabel *m_title;
    QProgressBar *m_progress;
    QTimeLine *m_showAnime, *m_hideAnime;
    QPalette m_progressPalette;
};

#endif // KNCOMPILEPROGRESS_H
