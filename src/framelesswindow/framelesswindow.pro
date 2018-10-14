include(../common.pri)
QMAKE_TARGET_PRODUCT = Qt Nice Frameless Window
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Frameless Window Module
QMAKE_TARGET_COMPANY = Bringer of Light
QMAKE_TARGET_COPYRIGHT = MIT
TARGET = framelesswindow
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= widgets
TEMPLATE = lib
DEFINES *= FRAMELESSWINDOW_LIBRARY
CONFIG *= dll
LIBS *= -lUser32
HEADERS += framelesswindow_global.h
include(../3rdparty/framelesswindow/framelesswindow.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
