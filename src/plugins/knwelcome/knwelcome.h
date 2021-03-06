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
#ifndef KNWELCOME_H
#define KNWELCOME_H

#include <QLinkedList>

#include "knwelcomebase.h"

class QBoxLayout;
class QLabel;
class QSignalMapper;
class KNWelcomeNewButton;
class KNWelcome : public KNWelcomeBase
{
    Q_OBJECT
public:
    explicit KNWelcome(QWidget *parent = 0);

signals:

public slots:

private slots:
    void retranslate();

private:
    inline void addNewButton(KNWelcomeNewButton *button);
    QLabel *m_banner, *m_newCaption, *m_openCaption;
    QBoxLayout *m_newLayout;
    QLinkedList<KNWelcomeNewButton *> m_newButtonList;
    QSignalMapper *m_newButtonMapper;
};

#endif // KNWELCOME_H
