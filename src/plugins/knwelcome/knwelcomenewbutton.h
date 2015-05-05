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

#ifndef KNWELCOMENEWBUTTON_H
#define KNWELCOMENEWBUTTON_H

#include <QLabel>

class QTimeLine;
class KNWelcomeNewButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNWelcomeNewButton(QWidget *parent = 0);
    QString suffix() const;

signals:
    void clicked();

public slots:
    void setSuffix(const QString &suffix);
    void setItemIcon(const QPixmap &icon);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionInOutAnime(int frame);

private:
    inline void startMouseInOutAnime(const int &targetFrame);
    bool m_pressed;
    QTimeLine *m_mouseInOutAnime;
    QLabel *m_itemIcon;
    QString m_suffix;
};

#endif // KNWELCOMENEWBUTTON_H
