include(../common.pri)
QMAKE_TARGET_PRODUCT = Player
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Player Module
TARGET = player
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
CONFIG *= dll
DEFINES *= BUILD_INNER_SHARED_LIBRARY_DD
QT *= widgets av avwidgets winextras
include(../ipc/ipc.pri)
include(../settingsmanager/settingsmanager.pri)
include(../utils/utils.pri)
include(../wallpaper/wallpaper.pri)
HEADERS += \
    $$PWD/../dd_inner_dll_global.h \
    player.h \
    mainwindow.h
SOURCES += \
    player.cpp \
    mainwindow.cpp
RESOURCES *= images.qrc
TRANSLATIONS += \
    ../resources/translations/player_en.ts \
    ../resources/translations/player_zh_CN.ts
include(../translations.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
