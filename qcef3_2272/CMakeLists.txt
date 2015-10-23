cmake_minimum_required(VERSION 3.2)
project(qt_cefclient)

set(CEF_VERSION "3.2272.32.gbda8dc7")

# TODO set arch

set(QT_HOME 'E:/Qt/Qt5.4.2/5.4/msvc2013_opengl')
#set(QT_HOME 'E:/Qt/Qt5.4.2/5.4/msvc2013_64_opengl')

set(CEF_BINARY "cef/cef_binary_${CEF_VERSION}_windows32")
#set(CEF_BINARY "cef/cef_binary_${CEF_VERSION}_windows64")

set(CONFIG "Release")

add_subdirectory(${CEF_BINARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CEFCLIENT_SOURCE
    cefclient/cefclient.cpp
    cefclient/cefclient.h
    cefclient/cefclient_qt.cpp
    cefclient/client_app.cpp
    cefclient/client_app.h
    cefclient/client_app_delegates.cpp
    cefclient/client_app_js.h
    cefclient/client_binding.cpp
    cefclient/client_binding.h
    cefclient/client_handler.cpp
    cefclient/client_handler.h
    cefclient/client_handler_qt.cpp
    cefclient/client_renderer.cpp
    cefclient/client_renderer.h
    cefclient/client_transfer.cpp
    cefclient/client_transfer.h
    cefclient/message_event.cpp
    cefclient/message_event.h
    cefclient/qcefwebview.cpp
    cefclient/qcefwebview.h
    cefclient/util.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    GeneratedFiles/${CONFIG}/moc_mainwindow.cpp
    GeneratedFiles/${CONFIG}/moc_qcefwebview.cpp
    GeneratedFiles/ui_mainwindow.h
)

set(CEFCLIENT_PROC_SOURCE
    cefclient/client_app.cpp
    cefclient/client_app.h
    cefclient/client_app_delegates.cpp
    cefclient/client_app_js.h
    cefclient/client_renderer.cpp
    cefclient/client_renderer.h
    cefclient/client_transfer.cpp
    cefclient/client_transfer.h
    cefclient/util.h
    main_process.cpp)

include_directories( SYSTEM "{CEF_BINARY}/include")
include_directories( SYSTEM "{CEF_BINARY}/cefclient")

ADD_CUSTOM_COMMAND(
    OUTPUT GeneratedFiles/${CONFIG}/moc_mainwindow.cpp
    COMMAND "${QT_HOME}\\bin\\moc.exe mainwindow.h -o GeneratedFiles\\${CONFIG}\\moc_mainwindow.cpp -DUNICODE -DWIN32 -DNOMINMAX -DQT_DLL -DQT_CORE_LIB -DQT_GUI_LIB -D_UNICODE -I. -I.\\GeneratedFiles -I.\\GeneratedFiles\\${CONFIG}\\. -I${CMAKE_CURRENT_SOURCE_DIR}\\. -I${QT_HOME}\\include -I${QT_HOME}\\include\\QtCore -I${QT_HOME}\\include\\QtGui"
    DEPENDS "mainwindow.h"
)
ADD_CUSTOM_COMMAND(
    OUTPUT GeneratedFiles/${CONFIG}/moc_qcefwebview.cpp
    COMMAND "${QT_HOME}\\bin\\moc.exe cefclient\\qcefwebview.h -o GeneratedFiles\\${CONFIG}\\moc_qcefwebview.cpp -DUNICODE -DWIN32 -DNOMINMAX -DQT_DLL -DQT_CORE_LIB -DQT_GUI_LIB -D_UNICODE  -I. -I.\\GeneratedFiles -I.\\GeneratedFiles\\${CONFIG}\\. -I${CMAKE_CURRENT_SOURCE_DIR}\\. -I${QT_HOME}\\include -I${QT_HOME}\\include\\QtCore -I${QT_HOME}\\include\\QtGui"
    DEPENDS "cefclient\\qcefwebview.h"
)
ADD_CUSTOM_COMMAND(
    OUTPUT GeneratedFiles/ui_mainwindow.h
    COMMAND "${QT_HOME}\\bin\\uic.exe -o GeneratedFiles\\ui_mainwindow.h mainwindow.ui"
    DEPENDS "mainwindow.ui"
)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CEF_BINARY}/build/libcef_dll/${CONFIG} ${CEF_BINARY}/${CONFIG})

add_executable(qt_cefclient_process ${CEFCLIENT_PROC_SOURCE})
add_dependencies(qt_cefclient_process libcef_dll_wrapper)

add_executable(qt_cefclient ${CEFCLIENT_SOURCE})
add_dependencies(qt_cefclient qt_cefclient_process)
