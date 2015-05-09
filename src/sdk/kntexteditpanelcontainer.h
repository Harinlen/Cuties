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

#ifndef KNTEXTEDITPANELCONTAINER_H
#define KNTEXTEDITPANELCONTAINER_H

#include <QLinkedList>

#include <QWidget>

class QBoxLayout;
class KNTextEdit;
class KNTextEditPanel;
class KNTextEditPanelContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNTextEditPanelContainer(QWidget *parent = 0);
    void addPanel(KNTextEditPanel *panel);
    int panelWidth();

signals:
    void requireResizeMargin();

public slots:
    void setEditor(KNTextEdit *editor);
    void updatePanels();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QBoxLayout *m_panelLayout;
    KNTextEdit *m_editor;
    QLinkedList<KNTextEditPanel *> m_panelList;
};

#endif // KNTEXTEDITPANELCONTAINER_H
