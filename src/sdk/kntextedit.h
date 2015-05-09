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

#ifndef KNTEXTEDIT_H
#define KNTEXTEDIT_H

#include <QPlainTextEdit>

class KNTextEditPanelContainer;
class KNTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit KNTextEdit(QWidget *parent = 0);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionUpdateMargins();
    void updateHighlights();

private:
    inline void highlightCurrentLine(QList<QTextEdit::ExtraSelection> &selections);
    KNTextEditPanelContainer *m_panelContainer;
    friend class KNTextEditPanel;

    QColor m_lineColor;
};

#endif // KNTEXTEDIT_H
