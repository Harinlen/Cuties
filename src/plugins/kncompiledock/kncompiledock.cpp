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
#include <QStandardItemModel>
#include <QBoxLayout>
#include <QTreeView>
#include <QAction>
#include <QToolButton>

#include "knglobal.h"
#include "kncodeeditor.h"
#include "kncompileprogress.h"
#include "knconnectionhandler.h"
#include "kncompileoutputreceiver.h"

#include "kncompiledock.h"

KNCompileDock::KNCompileDock(QWidget *parent) :
    KNCompileDockBase(parent),
    m_textOutput(new QPlainTextEdit(this)),
    m_treeViewOutput(new QTreeView(this)),
    m_editorHandles(new KNConnectionHandler(this)),
    m_compileProgress(new KNCompileProgress(this)),
    m_visible(new QAction(this))
{
    setObjectName("CompileDock");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
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
    m_actionButtons[0]=generateButton(":/image/resource/icons/compile/text_mode.png");
    m_actionButtons[0]->setChecked(true);
    m_actionButtons[1]=generateButton(":/image/resource/icons/compile/list_mode.png");
    buttonLayout->addWidget(m_actionButtons[0]);
    buttonLayout->addWidget(m_actionButtons[1]);
    buttonLayout->addStretch();
    contentLayout->addWidget(m_textOutput);
    contentLayout->addWidget(m_treeViewOutput);
    connect(m_actionButtons[0], &QToolButton::released,
            [=]
            {
                m_actionButtons[1]->setChecked(false);
                contentLayout->setCurrentWidget(m_textOutput);
            });
    connect(m_actionButtons[1], &QToolButton::released,
            [=]
            {
                m_actionButtons[0]->setChecked(false);
                contentLayout->setCurrentWidget(m_treeViewOutput);
            });

    //Configure text output.
    //  Properties
    m_textOutput->setMinimumHeight(0);
    m_textOutput->setFrameStyle(QFrame::NoFrame);
    m_textOutput->setReadOnly(true);
    m_textOutput->setWordWrapMode(QTextOption::NoWrap);
    //  Font
    QFont compileFont=m_textOutput->font();
    compileFont.setFamily("Monaco");
    m_textOutput->setFont(compileFont);
    //  Palette
    m_textOutput->setPalette(KNGlobal::instance()->getPalette("CompileDockTextOutput"));

    //Configure tree view output.
    //  Properties
    m_treeViewOutput->setIndentation(0);
    m_treeViewOutput->setMinimumHeight(0);
    m_treeViewOutput->setFrameStyle(QFrame::NoFrame);
    m_treeViewOutput->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_treeViewOutput->setHeaderHidden(true);
    //  Font
    m_treeViewOutput->setFont(compileFont);
    //  Palette
    m_treeViewOutput->setPalette(KNGlobal::instance()->getPalette("CompileDockTreeOutput"));

    //Configure the visible actions.
    m_visible->setIcon(QIcon(":/icon/resource/icons/actions/compile_dock.png"));
    connect(m_visible, SIGNAL(triggered()), this, SLOT(onActionChangeVisible()));

    //Link retranslate.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNCompileDock::retranslate);
    retranslate();
}

QAction *KNCompileDock::visibleControlAction()
{
    return m_visible;
}

QWidget *KNCompileDock::compileProgress()
{
    return m_compileProgress;
}

void KNCompileDock::setCodeEditor(KNCodeEditor *editor)
{
    //If there's previous connections
    if(!m_editorHandles->isEmpty())
    {
        //Clear the previous connection.
        m_editorHandles->disconnectAll();
        //Reset the treeview output.
        m_treeViewOutput->setModel(nullptr);
    }
    //Ignore the null editor.
    if(editor==nullptr)
    {
        return;
    }
    //Link the editor with compile progress.
    m_editorHandles->append(
                connect(editor, &KNCodeEditor::compileProgressChange,
                        m_compileProgress, &KNCompileProgress::onActionCompileProgressChange));
    m_editorHandles->append(
                connect(editor, &KNCodeEditor::requireShowCompileProgress,
                        m_compileProgress, &KNCompileProgress::showCompileProgress));
    m_editorHandles->append(
                connect(editor, &KNCodeEditor::requireHideCompileProgress,
                        m_compileProgress, &KNCompileProgress::hideCompileProgress));
    //Get the receiver of the editor.
    KNOutputReceiver *receiver=editor->outputReceiver();
    //Link the data change with the text output.
    m_editorHandles->append(
                connect(receiver, &KNOutputReceiver::compileOutputTextChange,
                        m_textOutput, &QPlainTextEdit::setPlainText));
    QStandardItemModel *outputModel=receiver->compileOutputModel();
    m_treeViewOutput->setModel(outputModel);
    m_editorHandles->append(
                connect(m_treeViewOutput, &QTreeView::doubleClicked,
                        [=](const QModelIndex &index)
                        {
                            //Ignore the invaild request.
                            if(index.data(Qt::UserRole+1).toInt()==-1)
                            {
                                return;
                            }
                            //Emit the goto signal.
                            emit requireGoto(index.data(Qt::UserRole+1).toString().toInt(),
                                             index.data(Qt::UserRole+2).toString().toInt());
                        }));
    m_editorHandles->append(
                connect(m_treeViewOutput->selectionModel(), &QItemSelectionModel::currentChanged,
                        [=](const QModelIndex & current, const QModelIndex & previous)
                        {
                            //If previous is available to use.
                            if(previous.isValid() &&
                                    previous.data(Qt::UserRole+1).toInt()!=-1)
                            {
                                //Reset the previous display data.
                                outputModel->setData(previous,
                                                     previous.data(Qt::UserRole+4),
                                                     Qt::DisplayRole);
                            }
                            //If the Qt::UserRole+1 is -1, means this index
                            //has no data to expand.
                            if(current.data(Qt::UserRole+1).toInt()==-1)
                            {
                                return;
                            }
                            //Change the current display data.
                            outputModel->setData(current,
                                                 current.data(Qt::UserRole+4).toString() + "\n"
                                                 + QString("  Line %1, Column %2\n").arg(
                                                     current.data(Qt::UserRole+1).toString(),
                                                     current.data(Qt::UserRole+2).toString())
                                                 + current.data(Qt::UserRole+5).toString(),
                                                 Qt::DisplayRole);
                        }));

    //Update the output data right now.
    m_textOutput->setPlainText(receiver->compileOutputText());
}

void KNCompileDock::retranslate()
{
    m_actionButtons[0]->setToolTip(tr("Text View"));
    m_actionButtons[1]->setToolTip(tr("List View"));

    m_visible->setText(tr("Compile Dock"));
}

void KNCompileDock::onActionChangeVisible()
{
    setVisible(!isVisible());
}

QToolButton *KNCompileDock::generateButton(const QString &iconPath)
{
    QToolButton *button=new QToolButton(this);
    button->setAutoRaise(true);
    button->setCheckable(true);
    button->setIcon(QIcon(iconPath));
    button->setPalette(KNGlobal::instance()->getPalette("CompileDockButton"));
    return button;
}
