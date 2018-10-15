include(../common.pri)
QMAKE_TARGET_PRODUCT = Utils
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Utils Module
TARGET = utils
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= widgets
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
LIBS *= \
    -lUser32 \
    -lShell32 \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
include(../qtservice/qtservice.pri)
include(../wallpaper/wallpaper.pri)
SOURCES += utils.cpp
HEADERS += \
    utils.h \
    $$PWD/../dd_dll_global.h
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
