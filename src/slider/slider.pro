include(../common.pri)
QMAKE_TARGET_PRODUCT = Slider
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Slider Module
TARGET = slider
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
QT *= widgets
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
SOURCES += slider.cpp
HEADERS += \
    slider.h \
    $$PWD/../dd_dll_global.h
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
