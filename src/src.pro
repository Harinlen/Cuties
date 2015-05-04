#
# Copyright (C) Kreogist Dev Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#

# Configure the GUI parts of the app

TEMPLATE = app
TARGET = cuties
INSTALLS += target

# Add modules.
QT += core \
      gui \
      widgets \

# Enabled c++ 11
CONFIG += c++11

# Add included path.
INCLUDEPATH += sdk

SOURCES += \
    main.cpp \
    sdk/knglobal.cpp \
    sdk/knmainwindow.cpp \
    sdk/knpluginmanager.cpp \
    sdk/knthememanager.cpp \
    sdk/knconfiguremanager.cpp \
    sdk/knconfigure.cpp \
    plugins/knwelcome/knwelcome.cpp \
    sdk/knsideshadowwidget.cpp

HEADERS += \
    sdk/knglobal.h \
    sdk/knmainwindow.h \
    sdk/knpluginmanager.h \
    sdk/knthememanager.h \
    sdk/knconfiguremanager.h \
    sdk/knconfigure.h \
    sdk/knversion.h \
    sdk/knwelcomebase.h \
    plugins/knwelcome/knwelcome.h \
    sdk/knsideshadowwidget.h

RESOURCES += \
    res.qrc
