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
#include "knglobal.h"

#include "knlabelanimebutton.h"
#include "kncodeeditor.h"
#include "kntextedit.h"
#include "kntabmanageritem.h"

#include <QDebug>

KNTabManagerItem::KNTabManagerItem(const QString &caption, QWidget *parent) :
    QLabel(parent),
    m_selected(false),
    m_codeEditor(new KNCodeEditor(this)),
    m_closeButton(new KNLabelAnimeButton(this))
{
    setObjectName("TabManagerItem");
    //Set properties.
    setContentsMargins(9,0,9,0);
    setFixedHeight(itemHeight());
    setAutoFillBackground(true);

    //Save and set the palette.
    m_palette=KNGlobal::instance()->getPalette(objectName());
    setPalette(m_palette);

    //Set the caption.
    setCaption(caption);
    //Configure the close button.
    m_closeButton->setPixmap(QPixmap(":/image/resource/images/close.png"));
    m_closeButton->setScaledContents(true);
    m_closeButton->resize(itemHeight(), itemHeight());
    m_closeButton->hide();
    connect(m_closeButton, &KNLabelAnimeButton::clicked,
            this, &KNTabManagerItem::onActionClose);

    //Link the code editor.
    connect(m_codeEditor, &KNCodeEditor::fileNameChange,
            this, &KNTabManagerItem::setCaption);
}

int KNTabManagerItem::itemHeight()
{
    return 24;
}

KNCodeEditor *KNTabManagerItem::codeEditor()
{
    return m_codeEditor;
}

void KNTabManagerItem::mousePressEvent(QMouseEvent *event)
{
    //Do the original pressed event.
    QLabel::mousePressEvent(event);
}

void KNTabManagerItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_selected)
    {
        //Set selected to true.
        setSelected(true);
        //When mouse released, emit the clicked signal.
        emit clicked();
    }
    //Do the release event.
    QLabel::mouseReleaseEvent(event);
}

void KNTabManagerItem::resizeEvent(QResizeEvent *event)
{
    //Resize the item.
    QLabel::resizeEvent(event);
    //Move the button.
    m_closeButton->move(width()-itemHeight(), 0);
}

void KNTabManagerItem::onActionPressed()
{
    ;
}

void KNTabManagerItem::onActionClose()
{
    //Check the modified status.
    if(m_codeEditor->textEditor()->document()->isModified())
    {
        //Ask user if they want to save the file.
        //!TODO: Add question ask code here.
        bool saveFile=false;
        if(saveFile)
        {
            m_codeEditor->saveFile();
        }
    }
    //Require to close the tab.
    emit requireCloseTab(this);
}

void KNTabManagerItem::setCaption(const QString &text)
{
    //Clear the old cache text.
    clearFileName();
    //Check the text is empty first.
    if(text.isEmpty())
    {
        //Clear the text.
        setText("");
        return;
    }
    //Find the suffix.
    int suffixStart=text.indexOf('.');
    if(suffixStart==-1)
    {
        //Cannot find suffix, just set the text.
        m_fileName=text;
        setText(text);
    }
    else
    {
        //Split the file name and suffix.
        m_fileName=text.left(suffixStart);
        m_suffix=text.mid(suffixStart);
        //Generate the caption text.
        setText(generateCaption());
    }
}

void KNTabManagerItem::setSelected(bool selected)
{
    //When the selected status is the same, ignore the request.
    if(selected==m_selected)
    {
        return;
    }
    //Save the value.
    m_selected=selected;
    //Change the style.
    QPalette pal=palette();
    if(m_selected)
    {
        //Change the background.
        pal.setBrush(QPalette::Window, m_palette.color(QPalette::Highlight));
        pal.setColor(QPalette::WindowText, m_palette.color(QPalette::HighlightedText));
        setPalette(pal);
        //Set the title.
        setText(m_fileName+m_suffix);
        return;
    }
    //Restore the background to transparent.
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, m_palette.color(QPalette::WindowText));
    setPalette(pal);
    setText(generateCaption());
    return;
}

void KNTabManagerItem::enterEvent(QEvent *event)
{
    //Show the close button.
    m_closeButton->show();
    //Execute the original enter event.
    QLabel::enterEvent(event);
}

void KNTabManagerItem::leaveEvent(QEvent *event)
{
    //Hide the close button.
    m_closeButton->hide();
    //Execute the leave event.
    QLabel::leaveEvent(event);
}

inline void KNTabManagerItem::clearFileName()
{
    m_fileName.clear();
    m_suffix.clear();
}

QString KNTabManagerItem::generateCaption()
{
    return m_fileName + "<font color='#94A3A7'>" + m_suffix + "</font>";
}
