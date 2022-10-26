INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogregister64.h \
    $$PWD/dialogregisterabstract.h \
    $$PWD/xregistersview.h \
    $$PWD/xregistersviewoptionswidget.h

SOURCES += \
    $$PWD/dialogregister64.cpp \
    $$PWD/dialogregisterabstract.cpp \
    $$PWD/xregistersview.cpp \
    $$PWD/xregistersviewoptionswidget.cpp

FORMS += \
    $$PWD/dialogregister64.ui \
    $$PWD/xregistersviewoptionswidget.ui

!contains(XCONFIG, xinfodb) {
    XCONFIG += xinfodb
    include($$PWD/../XInfoDB/xinfodb.pri)
}
