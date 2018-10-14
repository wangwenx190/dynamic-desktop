include(../common.pri)
QMAKE_TARGET_PRODUCT = QSimpleUpdater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater Core Module
QMAKE_TARGET_COMPANY = Alex Spataru
QMAKE_TARGET_COPYRIGHT = DON\'T BE A DICK PUBLIC LICENSE
TARGET = qsimpleupdater
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
DEFINES *= QSU_SHARED
CONFIG *= dll
HEADERS += qsimpleupdater_global.h
include(../3rdparty/qsimpleupdater/QSimpleUpdater.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
