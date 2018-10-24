include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop Player Module
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
RC_ICONS = ../resources/icons/color_palette.ico
TARGET = player
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets avwidgets winextras
include(../ipc/ipc.pri)
include(../settingsmanager/settingsmanager.pri)
include(../utils/utils.pri)
include(../wallpaper/wallpaper.pri)
HEADERS += mainwindow.h
SOURCES += \
    player.cpp \
    mainwindow.cpp
include(../deploy.pri)
