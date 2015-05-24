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
    sdk/knsideshadowwidget.cpp \
    sdk/knlocalemanager.cpp \
    sdk/knlanguagemanager.cpp \
    plugins/knwelcome/knwelcomenewbutton.cpp \
    sdk/kncodeeditor.cpp \
    sdk/kntabmanager.cpp \
    sdk/kntabmanageritem.cpp \
    sdk/kntextedit.cpp \
    sdk/kntexteditpanel.cpp \
    sdk/kntexteditpanelcontainer.cpp \
    plugins/kntexteditnumberpanel/kntexteditnumberpanel.cpp \
    sdk/kntextblockdata.cpp \
    sdk/knhighlighter.cpp \
    sdk/kncodestylemanager.cpp \
    sdk/knutilities.cpp \
    sdk/kncodeeditorunibar.cpp \
    sdk/knconnectionhandler.cpp \
    plugins/kntexteditmarkpanel/kntexteditmarkpanel.cpp \
    sdk/knlanguagemode.cpp \
    sdk/kncpphighlighter.cpp \
    sdk/knlabelanimebutton.cpp \
    sdk/knsidebarelement.cpp \
    sdk/knsidebar.cpp \
    sdk/knmainmenu.cpp \
    sdk/knsaostyle.cpp \
    sdk/knsaosubmenu.cpp \
    sdk/knsidebarcontent.cpp \
    sdk/knsidetabcontentcontainer.cpp \
    sdk/kntabmanagercontent.cpp \
    sdk/kncompiler.cpp \
    sdk/kngcccompiler.cpp \
    sdk/kngppcompiler.cpp \
    sdk/kncompileoutputreceiver.cpp

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
    sdk/knsideshadowwidget.h \
    sdk/knlocalemanager.h \
    sdk/knlanguagemanager.h \
    plugins/knwelcome/knwelcomenewbutton.h \
    sdk/kncodeeditor.h \
    sdk/kntabmanager.h \
    sdk/kntabmanageritem.h \
    sdk/kntextedit.h \
    sdk/kntexteditpanel.h \
    sdk/kntexteditpanelcontainer.h \
    plugins/kntexteditnumberpanel/kntexteditnumberpanel.h \
    sdk/kntextblockdata.h \
    sdk/knhighlighter.h \
    sdk/kncodestylemanager.h \
    sdk/knutilities.h \
    sdk/kncodeeditorunibar.h \
    sdk/knconnectionhandler.h \
    plugins/kntexteditmarkpanel/kntexteditmarkpanel.h \
    sdk/knlanguagemode.h \
    sdk/kncpphighlighter.h \
    sdk/knlabelanimebutton.h \
    sdk/knsidebarelement.h \
    sdk/knsidebar.h \
    sdk/knmainmenu.h \
    sdk/knsaostyle.h \
    sdk/knsaosubmenu.h \
    sdk/knsidebarcontent.h \
    sdk/knsidetabcontentcontainer.h \
    sdk/kntabmanagercontent.h \
    sdk/kncompiler.h \
    sdk/kngcccompiler.h \
    sdk/kngppcompiler.h \
    sdk/kncompileoutputreceiver.h

RESOURCES += \
    res.qrc
