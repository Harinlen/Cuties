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

#ifndef KNLABELANIMEBUTTON_H
#define KNLABELANIMEBUTTON_H

#include <QLabel>

class QTimeLine;
class KNLabelAnimeButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNLabelAnimeButton(QWidget *parent = 0);
    void setPalette(const QPalette &pal);
    bool checkable() const;
    void setCheckable(bool checkable);
    bool checked() const;
    void setChecked(bool checked);

signals:
    void clicked();
    void rightClicked();
    void entered();
    void leaved();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionUpdateBackground(const int &frame);

private:
    inline void startAnime(const int &end);
    QTimeLine *m_backgroundAnime;
    bool m_pressed, m_checkable, m_checked;
};

#endif // KNLABELANIMEBUTTON_H
