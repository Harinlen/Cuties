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
#include <QStandardItemModel>

#include "kncompileoutputreceiver.h"

#include <QDebug>

KNOutputReceiver::KNOutputReceiver(QObject *parent) :
    QObject(parent),
    m_compileOutputText(QString()),
    m_compileOutputModel(new QStandardItemModel(this))
{
}

QString KNOutputReceiver::compileOutputText() const
{
    return m_compileOutputText;
}

QStandardItemModel *KNOutputReceiver::compileOutputModel()
{
    return m_compileOutputModel;
}

void KNOutputReceiver::setCompileOutputText(const QString &outputText)
{
    m_compileOutputText = outputText;
    emit compileOutputTextChange(m_compileOutputText);
}

void KNOutputReceiver::appendCompileOutputText(const QString &text)
{
    m_compileOutputText.append(text);
    emit compileOutputTextChange(m_compileOutputText);
}

void KNOutputReceiver::appendCompileOutputItem(QStandardItem *item)
{
    //Add item to model.
    m_compileOutputModel->appendRow(item);
}

void KNOutputReceiver::clearCompileText()
{
    m_compileOutputText.clear();
    emit compileOutputTextChange(m_compileOutputText);
}

void KNOutputReceiver::clearCompileModel()
{
    //Clear the model.
    m_compileOutputModel->clear();
}

