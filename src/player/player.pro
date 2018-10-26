include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Player
RC_ICONS = ../resources/icons/color_palette.ico
TARGET = player
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets avwidgets
include(../ipc/ipc.pri)
include(../settingsmanager/settingsmanager.pri)
include(../utils/utils.pri)
include(../wallpaper/wallpaper.pri)
HEADERS += mainwindow.h
SOURCES += \
    player.cpp \
    mainwindow.cpp
TRANSLATIONS += \
    ../resources/translations/ply_en.ts \
    ../resources/translations/ply_zh_CN.ts
include(../translations.pri)
include(../deploy.pri)
