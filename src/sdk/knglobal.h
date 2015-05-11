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

#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QTextCharFormat>
#include <QPalette>

#include <QObject>

namespace KNGlobalNamespace
{
enum MainMenuCategory
{
    File,
    Edit,
    View,
    Search,
    Execute,
    Debug,
    Tools,
    Tabs,
    Help,
    MainMenuCategoryCount
};
}

using namespace KNGlobalNamespace;

class KNLanguageMode;
class KNLanguageManager;
class KNConfigureManager;
class KNLocaleManager;
class KNThemeManager;
class KNCodeStyleManager;
class KNLanguageManager;
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();
    QString simplifiedPath(const QString &path);
    QPalette getPalette(const QString &caption);
    QTextCharFormat getFormat(const QString &name);
    KNLanguageMode *getLanguageMode(const QString &suffix);
    static QString ensurePathAvaliable(const QString &path);
    void loadTheme(const QString &themeFilePath);
    void loadCodeStyle(const QString &codeStylePath);

signals:
    void languageUpdate();

public slots:

private:
    static KNGlobal *m_instance;
    explicit KNGlobal(QObject *parent = 0);

    inline void initialDefaultPath();

    KNConfigureManager *m_configureManager;
    KNThemeManager *m_themeManager;
    KNLocaleManager *m_localeManager;
    KNLanguageManager *m_languageManager;
    KNCodeStyleManager *m_codeStyleManager;

    QString m_userDataDir, m_resourceDir, m_configureDir;
};

#endif // KNGLOBAL_H
