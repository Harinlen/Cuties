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

#ifndef KNCOMPILEOUTPUTRECEIVER_H
#define KNCOMPILEOUTPUTRECEIVER_H

#include <QObject>

class KNCompileOutputReceiver : public QObject
{
    Q_OBJECT
public:
    explicit KNCompileOutputReceiver(QObject *parent = 0);

    QString outputText() const;
    void setOutputText(const QString &outputText);
    void clearOutputText();

signals:
    void outputTextChange(const QString &data);

public slots:

private:
    QString m_outputText;
};

#endif // KNCOMPILEOUTPUTRECEIVER_H
