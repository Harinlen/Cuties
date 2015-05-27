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
#include <QBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QAction>

#include "kncodeeditor.h"
#include "kntextedit.h"
#include "knlanguagemode.h"
#include "knglobal.h"
#include "knsidebar.h"
#include "knlabelanimebutton.h"
#include "knconnectionhandler.h"

#include "kncodeeditorunibar.h"

KNCodeEditorUnibar::KNCodeEditorUnibar(QWidget *parent) :
    QWidget(parent),
    m_editorConections(new KNConnectionHandler(this)),
    m_codeEditor(nullptr),
    m_textEditor(nullptr),
    m_fileName(new QLabel(this)),
    m_cursorPosition(new QLabel(this)),
    m_encoded(new QLabel(this)),
    m_language(new QLabel(this)),
    m_overwrite(new KNLabelAnimeButton(this)),
    m_close(new KNLabelAnimeButton(this)),
    m_borderColor(QColor(255,255,255,52))
{
    setObjectName("CodeEditorUnibar");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(26);
    setContentsMargins(0,0,0,0);

    //Set palette.
    QPalette unibarPal=KNGlobal::instance()->getPalette(objectName());
    setPalette(unibarPal);
    //Initial layout.
    QBoxLayout *borderLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    borderLayout->setContentsMargins(0,0,0,0);
    borderLayout->setSpacing(0);
    setLayout(borderLayout);

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          borderLayout->widget());
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    borderLayout->addLayout(mainLayout, 1);
    QFrame *borderLine=new QFrame(this);
    QPalette pal=borderLine->palette();
    pal.setColor(QPalette::WindowText, m_borderColor);
    borderLine->setPalette(pal);
    borderLine->setContentsMargins(0,0,0,0);
    borderLine->setFrameStyle(QFrame::HLine);
    borderLayout->addWidget(borderLine);

    //Configure cursor widget.
    m_cursorPosition->setContentsMargins(9,0,9,0);
    //Configure encoded widget.
    m_encoded->setContentsMargins(9,0,9,0);
    //Configure langauge widget.
    m_language->setContentsMargins(9,0,9,0);
    //Configure overwrite widget.
    m_overwrite->setContentsMargins(9,0,9,0);
    connect(m_overwrite, &KNLabelAnimeButton::clicked,
            [=]
            {
                if(m_textEditor!=nullptr)
                {
                    m_textEditor->setOverwriteMode(!m_textEditor->overwriteMode());
                }
            });
    //Configure close widget.
    m_close->setContentsMargins(0,0,0,0);
    m_close->setScaledContents(true);
    m_close->setFixedSize(25,25);
    m_close->setPixmap(QPixmap(":/image/resource/images/close.png"));
    connect(m_close, &KNLabelAnimeButton::clicked,
            this, &KNCodeEditorUnibar::requireCloseCurrent);

    mainLayout->addWidget(m_fileName, 1);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_cursorPosition);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_encoded);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_language);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_overwrite);
    mainLayout->addWidget(generateSeperateLine());
    mainLayout->addWidget(m_close);

    //Initial the actions.
    initialActions();

    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::languageUpdate,
            this, &KNCodeEditorUnibar::retranslate);
    retranslate();
}

void KNCodeEditorUnibar::setSidebar(KNSidebar *sidebar)
{
    //Get the main window, add all action.
    QWidget *mainWindow=sidebar->parentWidget();
    for(int i=0; i<UnibarActionCount; i++)
    {
        mainWindow->addAction(m_unibarActions[i]);
    }
    //Add to main menu.
    sidebar->addCategoryAction(Edit, m_unibarActions[Undo]);
    sidebar->addCategoryAction(Edit, m_unibarActions[Redo]);
    sidebar->addCategoryAction(Edit, m_unibarActions[Cut]);
    sidebar->addCategoryAction(Edit, m_unibarActions[Copy]);
    sidebar->addCategoryAction(Edit, m_unibarActions[Paste]);
    sidebar->addCategoryAction(Edit, m_unibarActions[SelectAll]);
}

KNCodeEditor *KNCodeEditorUnibar::editor() const
{
    return m_codeEditor;
}

void KNCodeEditorUnibar::setEditor(KNCodeEditor *editor)
{
    //Disconnect with the old connections.
    if(m_codeEditor!=nullptr)
    {
        //Clear the original status data.
        clearStatusData();
        //Disconnect all previous links.
        m_editorConections->disconnectAll();
        //Clear the pointer.
        m_codeEditor=nullptr;
        m_textEditor=nullptr;
    }
    //Set the editor.
    m_codeEditor = editor;
    if(m_codeEditor==nullptr)
    {
        return;
    }
    //Get the text edit.
    m_textEditor=m_codeEditor->textEditor();
    //Link the language mode change signal with the new editor.
    m_editorConections->append(
                connect(m_codeEditor, &KNCodeEditor::languageModeChange,
                        this, &KNCodeEditorUnibar::onActionLanguageChange));
    //Link the status display with the new editor.
    m_editorConections->append(
                connect(m_textEditor, &KNTextEdit::cursorPositionChanged,
                        this, &KNCodeEditorUnibar::onActionCursorPositionChange));
    m_editorConections->append(
                connect(m_textEditor, &KNTextEdit::overwriteModeChanged,
                        this, &KNCodeEditorUnibar::onActionUpdateOverwrite));
    //Link the action with the new editor.
    m_editorConections->append(
                connect(m_unibarActions[Undo], SIGNAL(triggered()),
                        m_textEditor, SLOT(undo())));
    m_editorConections->append(
                connect(m_unibarActions[Redo], SIGNAL(triggered()),
                        m_textEditor, SLOT(redo())));
    m_editorConections->append(
                connect(m_unibarActions[Cut], SIGNAL(triggered()),
                        m_textEditor, SLOT(cut())));
    m_editorConections->append(
                connect(m_unibarActions[Copy], SIGNAL(triggered()),
                        m_textEditor, SLOT(copy())));
    m_editorConections->append(
                connect(m_unibarActions[Paste], SIGNAL(triggered()),
                        m_textEditor, SLOT(paste())));
    m_editorConections->append(
                connect(m_unibarActions[SelectAll], SIGNAL(triggered()),
                        m_textEditor, SLOT(selectAll())));
    //Sync the data with the new editor.
    syncEditorStatusData();
}

void KNCodeEditorUnibar::retranslate()
{
    //Update position text.
    m_positionText=tr("Line %1, Row %2");
    //Update the overwrite text.
    m_overwriteStatus[false]=tr("Insert");
    m_overwriteStatus[true]=tr("Overwrite");
    //Sync the data with the new editor.
    syncEditorStatusData();

    //Update the actions' text.
    m_unibarActions[Undo]->setText(tr("Undo"));
    m_unibarActions[Redo]->setText(tr("Redo"));
    m_unibarActions[Cut]->setText(tr("Cut"));
    m_unibarActions[Copy]->setText(tr("Copy"));
    m_unibarActions[Paste]->setText(tr("Paste"));
    m_unibarActions[SelectAll]->setText(tr("Select All"));
}

void KNCodeEditorUnibar::onActionCursorPositionChange()
{
    //Clear the old data.
    m_cursorPosition->clear();
    //Ignore illegal request.
    if(m_textEditor==nullptr)
    {
        return;
    }
    m_cursorPosition->setText(m_positionText.arg(QString::number(m_textEditor->textCursor().blockNumber()+1),
                                                 QString::number(m_textEditor->textCursor().positionInBlock()+1)));
}

void KNCodeEditorUnibar::onActionUpdateOverwrite()
{
    //Clear the old status.
    m_overwrite->clear();
    //Check editor first.
    if(m_textEditor==nullptr)
    {
        return;
    }
    //Check the overwrite status.
    m_overwrite->setText(m_textEditor->overwriteMode()?
                             m_overwriteStatus[true]:m_overwriteStatus[false]);
}

void KNCodeEditorUnibar::onActionLanguageChange()
{
    //Clear the old language display.
    m_language->clear();
    //Check editor first.
    if(m_codeEditor==nullptr)
    {
        return;
    }
    //Set the langauge name.
    m_language->setText(m_codeEditor->languageMode()->languageName());
}

inline void KNCodeEditorUnibar::syncEditorStatusData()
{
    onActionCursorPositionChange();
    onActionUpdateOverwrite();
    onActionLanguageChange();
}

void KNCodeEditorUnibar::clearStatusData()
{
    //Clear cursor position data.
    m_cursorPosition->clear();
    //Clear codec data.
    m_encoded->clear();
    //Clear language data.
    m_language->clear();
    //Clear overwrite data.
    m_overwrite->clear();
}

void KNCodeEditorUnibar::initialActions()
{
    QString actionIcon[UnibarActionCount];
    actionIcon[Undo]=":/icon/resource/icons/actions/undo.png";
    actionIcon[Redo]=":/icon/resource/icons/actions/redo.png";
    actionIcon[Cut]=":/icon/resource/icons/actions/cut.png";
    actionIcon[Copy]=":/icon/resource/icons/actions/copy.png";
    actionIcon[Paste]=":/icon/resource/icons/actions/paste.png";
    actionIcon[SelectAll]=":/icon/resource/icons/actions/select_all.png";

    for(int i=0; i<UnibarActionCount; i++)
    {
        m_unibarActions[i]=new QAction(this);
        m_unibarActions[i]->setIcon(QIcon(actionIcon[i]));
        m_unibarActions[i]->setShortcutContext(Qt::WidgetShortcut);
    }

    //Set shortcuts.
    m_unibarActions[Undo]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    m_unibarActions[Redo]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));
    m_unibarActions[Cut]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
    m_unibarActions[Copy]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
    m_unibarActions[Paste]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
    m_unibarActions[SelectAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
}

QFrame *KNCodeEditorUnibar::generateSeperateLine()
{
    QFrame *line=new QFrame(this);
    line->setContentsMargins(0,0,0,0);
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, m_borderColor);
    line->setFrameStyle(QFrame::VLine);
    line->setPalette(pal);
    return line;
}

