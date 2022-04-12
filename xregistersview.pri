INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogregister64.h \
    $$PWD/xregistersview.h

SOURCES += \
    $$PWD/dialogregister64.cpp \
    $$PWD/xregistersview.cpp

!contains(XCONFIG, xinfodb) {
    XCONFIG += xinfodb
    include($$PWD/../XInfoDB/xinfodb.pri)
}

FORMS += \
    $$PWD/dialogregister64.ui
