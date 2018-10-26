include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Settings Manager Module
TARGET = settingsmanager
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT -= gui
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
SOURCES += settingsmanager.cpp
HEADERS += \
    settingsmanager.h \
    $$PWD/../dd_dll_global.h \
    $$PWD/../common.h
include(../deploy.pri)
