include(../common.pri)
QMAKE_TARGET_PRODUCT = Player
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Multimedia Player
RC_ICONS = ../resources/icons/dll_win10.ico
TARGET = player
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets av avwidgets
include(../qtsingleapplication/qtsingleapplication.pri)
include(../settingsmanager/settingsmanager.pri)
include(../utils/utils.pri)
HEADERS += mainwindow.h
SOURCES += \
    main.cpp \
    mainwindow.cpp
TRANSLATIONS += \
    ../resources/translations/player_en.ts \
    ../resources/translations/player_zh_CN.ts
include(../translations.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
