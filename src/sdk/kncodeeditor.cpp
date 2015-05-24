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
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>

#include "knglobal.h"
#include "kntextedit.h"
#include "knlanguagemode.h"
#include "kncompiler.h"
#include "kncompileoutputreceiver.h"
#include "knhighlighter.h"

#include "kncodeeditor.h"

KNCodeEditor::KNCodeEditor(QWidget *parent) :
    QWidget(parent),
    m_editor(new KNTextEdit(this)),
    m_languageMode(nullptr),
    m_filePath(QString()),
    m_codec(QString("UTF-8")),
    m_compileOutput(new KNCompileOutputReceiver(this))
{
    setObjectName("CodeEditor");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    //Set palette.
    setPalette(KNGlobal::instance()->getPalette(objectName()));
    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    mainLayout->addWidget(m_editor, 1);

    //Link the editor to code editor.
    connect(m_editor->document(), &QTextDocument::modificationChanged,
            this, &KNCodeEditor::modificationChanged);

    //Set the default language mode.
    setLanguageMode("");
}

KNCodeEditor::~KNCodeEditor()
{
    //Clear the language mode and recover the memory.
    clearLanguageMode();
}

KNLanguageMode *KNCodeEditor::languageMode() const
{
    return m_languageMode;
}

void KNCodeEditor::openFile(const QString &filePath)
{
    //Try to open the file.
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //Generate the text stream.
        QTextStream textIn(&file);
        //Configure the text in stream.
        textIn.setCodec(QTextCodec::codecForName(m_codec.toUtf8()));

        //Read data.
        m_editor->setPlainText(QString(textIn.readAll()));

        //Close the file.
        file.close();
    }
}

void KNCodeEditor::saveFile()
{
    //Check the file path. Ignore the request when it's empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Try to open the file in write only mode.
    QFile file(m_filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //Generate the text stream.
        QTextStream textOut(&file);
        //Configure the text out stream.
        textOut.setCodec(QTextCodec::codecForName(m_codec.toUtf8()));
        //Output data to stream & flush.
        textOut << m_editor->toPlainText() << flush;
        //Close the file
        file.close();

        //Set the modification to false.
        m_editor->document()->setModified(false);
    }
}

void KNCodeEditor::setLanguageMode(KNLanguageMode *languageMode)
{
    //Clear the old language mode.
    clearLanguageMode();
    //Set new language mode.
    m_languageMode = languageMode;
    //Link the language mode.
    m_languageMode->highlighter()->setDocument(m_editor->document());
    //Emit the language mode changed signal.
    emit languageModeChange();
}

inline void KNCodeEditor::clearLanguageMode()
{
    if(m_languageMode)
    {
        //Delete the original language mode.
        delete m_languageMode;
        //Reset the language mode.
        m_languageMode=nullptr;
    }
}

QString KNCodeEditor::encoded() const
{
    return m_codec;
}

void KNCodeEditor::setEncoded(const QString &encoded)
{
    //Save the encoded.
    m_codec = encoded;
}

KNCompileOutputReceiver *KNCodeEditor::compileOutput()
{
    return m_compileOutput;
}

void KNCodeEditor::compile()
{
    //Check if compiler is nullptr.
    if(m_languageMode->compiler()==nullptr ||
            m_filePath.isEmpty()) //When the file path is empty, ignore it.
    {
        //Ignore the compile command.
        return;
    }
    //Check the compile output.
    ;
    //Reset the output data.
//    m_compileOutput;
    //Compile file.
    m_languageMode->compiler()->compile(m_filePath);
}

QString KNCodeEditor::filePath() const
{
    return m_filePath;
}

void KNCodeEditor::setFilePath(const QString &filePath)
{
    //Save the changed file path.
    m_filePath = filePath;
    //Get the detail information.
    QFileInfo currentFile(m_filePath);
    //Change the langauge mode.
    setLanguageMode(currentFile.suffix());
    //Emit file name changed signal.
    emit fileNameChange(currentFile.fileName());
}


KNTextEdit *KNCodeEditor::textEditor() const
{
    return m_editor;
}

void KNCodeEditor::setLanguageMode(const QString &suffix)
{
    //Set language mode according to suffix.
    setLanguageMode(KNGlobal::instance()->getLanguageMode(suffix));
}
