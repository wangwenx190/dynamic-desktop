include(../common.pri)
QMAKE_TARGET_PRODUCT = Updater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater
RC_ICONS = ../resources/icons/dll_win10.ico
TARGET = updater
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets
include(../qsimpleupdater/qsimpleupdater.pri)
SOURCES += main.cpp
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
