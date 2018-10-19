include(../common.pri)
QMAKE_TARGET_PRODUCT = Updater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater Module
TARGET = updater
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
CONFIG *= dll
DEFINES *= BUILD_INNER_SHARED_LIBRARY_DD
QT *= widgets
include(../qsimpleupdater/qsimpleupdater.pri)
include(../utils/utils.pri)
HEADERS += \
    $$PWD/../dd_inner_dll_global.h \
    updater.h
SOURCES += updater.cpp
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
