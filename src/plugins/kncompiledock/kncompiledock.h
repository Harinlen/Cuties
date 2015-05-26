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

#ifndef KNCOMPILEDOCK_H
#define KNCOMPILEDOCK_H

#include "kncompiledockbase.h"

class QTreeView;
class QToolButton;
class QPlainTextEdit;
class KNConnectionHandler;
class KNCompileProgress;
class KNCompileDock : public KNCompileDockBase
{
    Q_OBJECT
public:
    explicit KNCompileDock(QWidget *parent = 0);
    QAction *visibleControlAction();
    QWidget *compileProgress();

signals:

public slots:
    void setCodeEditor(KNCodeEditor *editor);

private slots:
    void retranslate();
    void onActionChangeVisible();

private:
    inline QToolButton *generateButton(const QString &iconPath);
    QToolButton *m_actionButtons[2];
    QPlainTextEdit *m_textOutput;
    QTreeView *m_treeViewOutput;
    KNConnectionHandler *m_editorHandles;
    KNCompileProgress *m_compileProgress;

    QAction *m_visible;
};

#endif // KNCOMPILEDOCK_H
