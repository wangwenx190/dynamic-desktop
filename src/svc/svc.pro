include(../common.pri)
QMAKE_TARGET_PRODUCT = Service
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Startup Service
RC_ICONS = images/gear.ico
TARGET = ddsvc
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT -= gui widgets
CONFIG *= console
include(../qtservice/qtservice.pri)
include(../utils/utils.pri)
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
SOURCES += main.cpp
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
