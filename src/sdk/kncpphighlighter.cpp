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

#include "kncpphighlighter.h"

KNCppHighlighter::KNCppHighlighter(QObject *parent) :
    KNHighlighter(parent)
{
    //Add cpp rules.
    addRule("datatype",
            "\\b(bool|char|double|float|int|long|short|signed|unsigned|void|"
            "wchar_t|char16_t|char32_t|nullptr)\\b");
    addRule("keyword",
            "\\b(__asm|__cdecl|__declspec|__export|__far16|"
            "__fastcall|__fortran|__import|"
            "__pascal|__rtti|__stdcall|_asm|_cdecl|"
            "__except|_export|_far16|_fastcall|"
            "__finally|_fortran|_import|_stdcall|__thread|__try|asm|auto|"
            "break|case|catch|cdecl|const|continue|default|"
            "do|else|enum|extern|for|goto|"
            "if|register|return|sizeof|static|"
            "struct|switch|"
            "typedef|union|"
            "volatile|while|"
            "class|const_cast|delete|"
            "dynamic_cast|explicit|false|friend|"
            "inline|mutable|namespace|new|operator|private|protected|"
            "public|reinterpret_cast|static_cast|"
            "template|this|throw|true|"
            "try|typeid|typename|"
            "using|virtual)\\b");
    addRule("number",
            "\\b\\d+(\\.)?\\d*\\b");
    addRule("preprocdata",
            "^[[:blank:]]*#([[:blank:]]*[[:word:]]*).*");
    addRule("preproc",
            "^[[:blank:]]*#([[:blank:]]*[[:word:]]*)");
    addRule("todo",
            "(TODO|FIXME|BUG)([:]?)");
}
