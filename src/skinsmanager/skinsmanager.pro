include(../common.pri)
QMAKE_TARGET_PRODUCT = Skins Manager
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Skins Manager Module
TARGET = skinsmanager
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
QT *= widgets
DEFINES *= SKINSMANAGER_LIBRARY
CONFIG *= dll
SOURCES += skinsmanager.cpp
HEADERS += \
    skinsmanager.h \
    skinsmanager_global.h
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
