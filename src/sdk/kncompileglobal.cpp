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

#include "kncompileglobal.h"

KNCompileGlobal *KNCompileGlobal::m_instance=nullptr;

KNCompileGlobal *KNCompileGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNCompileGlobal:m_instance;
}

KNCompileGlobal::KNCompileGlobal()
{
    m_warningIcon=QPixmap(":/image/resource/icons/compile/warning.png");
    m_errorIcon=QPixmap(":/image/resource/icons/compile/error.png");
}

QPixmap KNCompileGlobal::warningIcon() const
{
    return m_warningIcon;
}

void KNCompileGlobal::setWarningIcon(const QPixmap &warningIcon)
{
    m_warningIcon = warningIcon;
}

QPixmap KNCompileGlobal::errorIcon() const
{
    return m_errorIcon;
}

void KNCompileGlobal::setErrorIcon(const QPixmap &errorIcon)
{
    m_errorIcon = errorIcon;
}
