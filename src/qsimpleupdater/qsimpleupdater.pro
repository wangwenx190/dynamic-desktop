include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = QSimpleUpdater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
QMAKE_TARGET_COMPANY = Alex Spataru
QMAKE_TARGET_COPYRIGHT = DON\'T BE A DICK PUBLIC LICENSE
TARGET = qsimpleupdater
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
HEADERS += $$PWD/../dd_dll_global.h
include(../3rdparty/qsimpleupdater/qsimpleupdater.pri)
include(../deploy.pri)
