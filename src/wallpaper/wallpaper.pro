include(../common.pri)
QMAKE_TARGET_PRODUCT = Wallpaper
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Wallpaper Module
QMAKE_TARGET_COMPANY = ThomasHuai
QMAKE_TARGET_COPYRIGHT = Apache License 2.0
TARGET = wallpaper
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
CONFIG -= qt
LIBS *= -lUser32
HEADERS += $$PWD/../dd_dll_global.h
include(../3rdparty/wallpaper/wallpaper.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
