include(../common.pri)
QMAKE_TARGET_PRODUCT = Qt Service
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Service Core Module
QMAKE_TARGET_COMPANY = The Qt Company
QMAKE_TARGET_COPYRIGHT = LGPLv2.1
TARGET = qtservice
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT -= gui
TEMPLATE = lib
DEFINES *= QTSERVICE_LIBRARY
CONFIG *= dll
HEADERS += qtservice_global.h
include(../3rdparty/qtservice/qtservice.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
