include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Service
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Service Module
TARGET = service
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
SOURCES += service.cpp
include(../deploy.pri)
