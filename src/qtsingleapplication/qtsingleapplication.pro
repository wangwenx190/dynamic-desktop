include(../common.pri)
QMAKE_TARGET_PRODUCT = Qt Single Application
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Single Application Module
QMAKE_TARGET_COMPANY = The Qt Company
QMAKE_TARGET_COPYRIGHT = LGPLv2.1
TARGET = qtsingleapplication
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
HEADERS += $$PWD/../dd_dll_global.h
include(../3rdparty/qtsingleapplication/qtsingleapplication.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
