include(../common.pri)
QMAKE_TARGET_PRODUCT = Qt Nice Frameless Window
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Frameless Window Module
QMAKE_TARGET_COMPANY = Bringer of Light
QMAKE_TARGET_COPYRIGHT = MIT
TARGET = qtniceframelesswindow
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= widgets
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
LIBS *= -lUser32
HEADERS += $$PWD/../dd_dll_global.h
include(../3rdparty/qtniceframelesswindow/qtniceframelesswindow.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
