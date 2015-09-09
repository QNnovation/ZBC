QT += widgets

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    zbcmainwindow.h \
    zbcpushbutton.h \
    zbcnewfolder.h \
    zbcsideframe.h \
    zbccentralwidget.h \
    wgttextview.h \
    wgtcopy.h \
    wgtcopy_p.h \
    zbc_newfolder.h \
    wgtfindreplacetext.h
    zbc_newfolder.h

SOURCES += \
    zbcmainwindow.cpp \
    zbcpushbutton.cpp \
    zbcnewfolder.cpp \
    zbcsideframe.cpp \
    zbccentralwidget.cpp \
    wgttextview.cpp \
    wgtcopy.cpp \
    main.cpp \
    zbc_newfolder.cpp \
    wgtfindreplacetext.cpp
