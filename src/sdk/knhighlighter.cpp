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
#include "kntextblockdata.h"

#include "knhighlighter.h"

KNHighlighter::KNHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    ;
}

void KNHighlighter::addRules(const QString &name,
                             const QString &pattern)
{
    SyntaxRule rule;
    rule.name=name;
    rule.express=QRegularExpression(pattern);
    //Add pattern to rules.
    m_rules.append(rule);
}

void KNHighlighter::highlightBlock(const QString &text)
{
    //Get the current user data, if we don't have one, generate one.
    KNTextBlockData *data=
            static_cast<KNTextBlockData *>(currentBlockUserData());
    if(nullptr==data)
    {
        //Generate block data.
        data=new KNTextBlockData;
        setCurrentBlockUserData(data);
    }

    //Do highlight.
    syntaxHighlight(text);
}

void KNHighlighter::syntaxHighlight(const QString &text)
{
    for(QLinkedList<SyntaxRule>::iterator i=m_rules.begin();
        i!=m_rules.end();
        ++i)
    {
        QRegularExpressionMatchIterator matchIterator=(*i).express.globalMatch(text);
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match=matchIterator.next();
            setFormat(match.capturedStart(),
                      match.capturedLength(),
                      KNGlobal::instance()->getFormat((*i).name));
        }
    }
}
