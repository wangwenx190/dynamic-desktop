include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop Skins Manager Module
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
TARGET = skinsmanager
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
QT *= widgets
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
SOURCES += skinsmanager.cpp
HEADERS += \
    skinsmanager.h \
    $$PWD/../dd_dll_global.h
include(../deploy.pri)
