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

#ifndef KNOUTPUTRECEIVER_H
#define KNOUTPUTRECEIVER_H

#include <QObject>

class QStandardItem;
class QStandardItemModel;
class KNOutputReceiver : public QObject
{
    Q_OBJECT
public:
    explicit KNOutputReceiver(QObject *parent = 0);

    QString compileOutputText() const;
    QStandardItemModel *compileOutputModel();

signals:
    void compileOutputTextChange(const QString &data);

public slots:
    void appendCompileOutputText(const QString &text);
    void appendCompileOutputItem(QStandardItem *item);
    void clearCompileText();
    void clearCompileModel();
    void setCompileOutputText(const QString &compileOutputText);

private:
    //Output datas.
    QString m_compileOutputText;
    QStandardItemModel *m_compileOutputModel;
};

#endif // KNOUTPUTRECEIVER_H
