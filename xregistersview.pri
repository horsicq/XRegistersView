INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogregistergeneral.h \
    $$PWD/xregistersview.h \
    $$PWD/xregistersviewoptionswidget.h

SOURCES += \
    $$PWD/dialogregistergeneral.cpp \
    $$PWD/xregistersview.cpp \
    $$PWD/xregistersviewoptionswidget.cpp

!contains(XCONFIG, xinfodb) {
    XCONFIG += xinfodb
    include($$PWD/../XInfoDB/xinfodb.pri)
}

FORMS += \
    $$PWD/dialogregistergeneral.ui \
    $$PWD/xregistersviewoptionswidget.ui
