INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogdebug_x86.h \
    $$PWD/dialogregister16.h \
    $$PWD/dialogregister32.h \
    $$PWD/dialogregister64.h \
    $$PWD/dialogregisterabstract.h \
    $$PWD/xregistersview.h \
    $$PWD/xregistersviewoptionswidget.h

SOURCES += \
    $$PWD/dialogdebug_x86.cpp \
    $$PWD/dialogregister16.cpp \
    $$PWD/dialogregister32.cpp \
    $$PWD/dialogregister64.cpp \
    $$PWD/dialogregisterabstract.cpp \
    $$PWD/xregistersview.cpp \
    $$PWD/xregistersviewoptionswidget.cpp

FORMS += \
    $$PWD/dialogdebug_x86.ui \
    $$PWD/dialogregister16.ui \
    $$PWD/dialogregister32.ui \
    $$PWD/dialogregister64.ui \
    $$PWD/xregistersviewoptionswidget.ui

!contains(XCONFIG, xinfodb) {
    XCONFIG += xinfodb
    include($$PWD/../XInfoDB/xinfodb.pri)
}
