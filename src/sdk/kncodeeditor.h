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

#ifndef KNCODEEDITOR_H
#define KNCODEEDITOR_H

#include <QWidget>

class KNTextEdit;
class KNLanguageMode;
class KNCodeEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNCodeEditor(QWidget *parent = 0);
    ~KNCodeEditor();
    KNLanguageMode *languageMode() const;
    void openFile(const QString &filePath);
    void saveFile();

    KNTextEdit *textEditor() const;

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QString encoded() const;
    void setEncoded(const QString &encoded);

signals:
    void fileNameChange(const QString &fileName);
    void modificationChanged(bool c);
    void languageModeChange();

public slots:
    void setLanguageMode(const QString &suffix);
    void setLanguageMode(KNLanguageMode *languageMode);

private:
    inline void clearLanguageMode();
    KNTextEdit *m_editor;
    KNLanguageMode *m_languageMode;

    QString m_filePath, m_codec;
};

#endif // KNCODEEDITOR_H
