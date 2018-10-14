include(../common.pri)
QMAKE_TARGET_PRODUCT = Settings Manager
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Settings Manager Module
TARGET = settingsmanager
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT -= gui
TEMPLATE = lib
DEFINES *= SETTINGSMANAGER_LIBRARY
CONFIG *= dll
SOURCES += settingsmanager.cpp
HEADERS += \
    settingsmanager.h \
    settingsmanager_global.h
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
