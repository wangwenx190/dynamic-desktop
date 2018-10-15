include(../common.pri)
QMAKE_TARGET_PRODUCT = Skins Manager
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Skins Manager Module
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
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)