include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../XInfoDB/xinfodb.cmake)

set(XREGISTERSVIEW_SOURCES
    ${XINFODB_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogdebug_x86.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister16.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister32.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister64.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogregisterabstract.h
    ${CMAKE_CURRENT_LIST_DIR}/xregistersview.h
    ${CMAKE_CURRENT_LIST_DIR}/xregistersviewoptionswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogdebug_x86.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister16.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister32.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister64.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogregisterabstract.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xregistersview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xregistersviewoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogdebug_x86.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister16.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister32.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogregister64.ui
    ${CMAKE_CURRENT_LIST_DIR}/xregistersviewoptionswidget.ui
)
