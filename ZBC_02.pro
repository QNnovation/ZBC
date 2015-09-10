QT += widgets

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

HEADERS += \
    zbcmainwindow.h \
    zbccentralwidget.h \
    wgttextview.h \
    wgtcopy.h \
    wgtcopy_p.h \
    zbc_newfolder.h \
    wgtfindreplacetext.h \
    zbc_sideframe.h \
    zbc_pushbutton.h
    zbc_newfolder.h

SOURCES += \
    zbcmainwindow.cpp \
    zbccentralwidget.cpp \
    wgttextview.cpp \
    wgtcopy.cpp \
    main.cpp \
    zbc_newfolder.cpp \
    wgtfindreplacetext.cpp \
    zbc_sideframe.cpp \
    zbc_pushbutton.cpp
