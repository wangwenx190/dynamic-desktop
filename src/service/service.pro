include(../common.pri)
QMAKE_TARGET_PRODUCT = Service
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Service Module
TARGET = service
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
CONFIG *= dll
DEFINES *= BUILD_INNER_SHARED_LIBRARY_DD
QT -= gui widgets
CONFIG *= console
include(../qtservice/qtservice.pri)
include(../utils/utils.pri)
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
HEADERS += \
    $$PWD/../dd_inner_dll_global.h \
    service.h
SOURCES += service.cpp
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
