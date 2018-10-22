include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Player
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Player Module
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
