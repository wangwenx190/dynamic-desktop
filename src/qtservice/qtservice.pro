include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Qt Service
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
QMAKE_TARGET_COMPANY = The Qt Company
QMAKE_TARGET_COPYRIGHT = LGPLv2.1
TARGET = qtservice
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT -= gui
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
HEADERS += $$PWD/../dd_dll_global.h
include(../3rdparty/qtservice/qtservice.pri)
include(../deploy.pri)
