include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop Updater Module
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
RC_ICONS = ../resources/icons/dll_win10.ico
TARGET = updater
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets
include(../qsimpleupdater/qsimpleupdater.pri)
include(../utils/utils.pri)
SOURCES += updater.cpp
include(../deploy.pri)
