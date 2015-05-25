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

#ifndef KNCOMPILEGLOBAL_H
#define KNCOMPILEGLOBAL_H

#include <QPixmap>

class KNCompileGlobal
{
public:
    static KNCompileGlobal *instance();
    QPixmap errorIcon() const;
    void setErrorIcon(const QPixmap &errorIcon);
    QPixmap warningIcon() const;
    void setWarningIcon(const QPixmap &warningIcon);

private:
    static KNCompileGlobal *m_instance;
    KNCompileGlobal();

    QPixmap m_errorIcon, m_warningIcon;
};

#endif // KNCOMPILEGLOBAL_H
