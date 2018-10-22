include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Updater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater Module
TARGET = updater
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets
include(../qsimpleupdater/qsimpleupdater.pri)
include(../utils/utils.pri)
SOURCES += updater.cpp
include(../deploy.pri)
