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
#include <QPlainTextEdit>
#include <QStackedLayout>
#include <QBoxLayout>

#include "knglobal.h"
#include "knconnectionhandler.h"
#include "kncompileoutputreceiver.h"

#include "kncompiledock.h"

KNCompileDock::KNCompileDock(QWidget *parent) :
    KNCompileDockBase(parent),
    m_textOutput(new QPlainTextEdit(this)),
    m_receiverHandles(new KNConnectionHandler(this))
{
    //Set properties.
    setMinimumHeight(160);
    //Set the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Set the button and stack layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    buttonLayout->setContentsMargins(0,0,0,0);
    buttonLayout->setSpacing(0);
    mainLayout->addLayout(buttonLayout);

    QStackedLayout *contentLayout=new QStackedLayout(mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(contentLayout ,1);

    //Add widgets.
    contentLayout->addWidget(m_textOutput);

    //Configure text output.
    //  Properties
    m_textOutput->setFrameStyle(QFrame::NoFrame);
    m_textOutput->setReadOnly(true);
    //  Palette
    m_textOutput->setPalette(KNGlobal::instance()->getPalette("CompileDockTextOutput"));
}

void KNCompileDock::setOutputReceiver(KNCompileOutputReceiver *receiver)
{
    //If there's previous connections, then clear the previous connection.
    if(!m_receiverHandles->isEmpty())
    {
        m_receiverHandles->disconnectAll();
    }

    //Link the data change with the text output.
    m_receiverHandles->append(
                connect(receiver, &KNCompileOutputReceiver::compileOutputTextChange,
                        m_textOutput, &QPlainTextEdit::setPlainText));

    //Update the data right now.
    m_textOutput->setPlainText(receiver->compileOutputText());
}
