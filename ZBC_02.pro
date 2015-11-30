win32:RC_FILE = file.rc

QT += widgets

CONFIG += c++11

QT += printsupport

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

HEADERS += \
    wgttextview.h \
    wgtcopy.h \
    wgtcopy_p.h \
    zbc_newfolder.h \
    wgtfindreplacetext.h \
    zbc_sideframe.h \
    zbc_pushbutton.h \
    zbc_centralwidget.h \
    zbc_centralwidget_p.h \
    zbc_treeview.h \
    wgtfilessearch.h \
    zbc_mainwindow.h \
    zbc_lineedit.h \
    zbc_drivebutton.h
    zbc_newfolder.h

SOURCES += \
    wgttextview.cpp \
    wgtcopy.cpp \
    main.cpp \
    zbc_newfolder.cpp \
    wgtfilessearch.cpp \
    wgtfindreplacetext.cpp \
    zbc_sideframe.cpp \
    zbc_pushbutton.cpp \
    zbc_centralwidget.cpp \
    zbc_treeview.cpp \
    zbc_mainwindow.cpp \
    zbc_lineedit.cpp \
    zbc_drivebutton.cpp

RESOURCES +=
