QT += widgets

CONFIG += c++11

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
    zbc_mainwindow.h \
    zbc_lineedit.h \
    zbc_drivebutton.h \
    zbc_drivebuttonswidget.h
    zbc_newfolder.h

SOURCES += \
    wgttextview.cpp \
    wgtcopy.cpp \
    main.cpp \
    zbc_newfolder.cpp \
    wgtfindreplacetext.cpp \
    zbc_sideframe.cpp \
    zbc_pushbutton.cpp \
    zbc_centralwidget.cpp \
    zbc_treeview.cpp \
    zbc_mainwindow.cpp \
    zbc_lineedit.cpp \
    zbc_drivebutton.cpp \
    zbc_drivebuttonswidget.cpp

RESOURCES += \
    res.qrc
