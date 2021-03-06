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

#ifndef KNCOMPILEDOCKBASE_H
#define KNCOMPILEDOCKBASE_H

#include <QWidget>

class QAction;
class KNCodeEditor;
class KNOutputReceiver;
class KNCompileDockBase : public QWidget
{
    Q_OBJECT
public:
    KNCompileDockBase(QWidget *parent = 0):QWidget(parent){}
    virtual QAction *visibleControlAction()=0;
    virtual QWidget *compileProgress()=0;

signals:
    void requireGoto(int line, int column);

public slots:
    virtual void setCodeEditor(KNCodeEditor *editor)=0;
};

#endif // KNCOMPILEDOCKBASE_H
