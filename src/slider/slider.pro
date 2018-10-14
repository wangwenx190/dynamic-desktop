include(../common.pri)
QMAKE_TARGET_PRODUCT = Slider
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Slider Module
TARGET = slider
contains(QT_ARCH, x86_64): TARGET = $$join(TARGET,,,64)
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
QT *= widgets
DEFINES *= SLIDER_LIBRARY
CONFIG *= dll
SOURCES += slider.cpp
HEADERS += \
    slider.h \
    slider_global.h
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
