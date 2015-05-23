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

#ifndef KNTABMANAGERITEM_H
#define KNTABMANAGERITEM_H

#include <QLabel>

class KNCodeEditor;
class KNLabelAnimeButton;
class KNTabManagerItem : public QLabel
{
    Q_OBJECT
public:
    explicit KNTabManagerItem(const QString &caption = "", QWidget *parent = 0);
    static int itemHeight();
    KNCodeEditor *codeEditor();

signals:
    void clicked();
    void requireCloseTab(KNTabManagerItem *item);

public slots:
    void setCaption(const QString &text);
    void setSelected(bool selected);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionPressed();
    void onActionClose();

private:
    inline void clearFileName();
    inline QString generateCaption();
    QString m_fileName, m_suffix;
    QPalette m_palette;
    bool m_selected;
    KNCodeEditor *m_codeEditor;
    KNLabelAnimeButton *m_closeButton;
};

#endif // KNTABMANAGERITEM_H
