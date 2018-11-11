TARGET = ddservice
include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Auto Start Service"
CONFIG(enable_lite_build): RC_ICONS = ../resources/icons/color_palette_small.ico
else: RC_ICONS = ../resources/icons/color_palette.ico
CONFIG -= qt
CONFIG *= console
LIBS *= -lAdvapi32
include(../ddutils/ddutils.pri)
TEMPLATE = app
SOURCES += main.cpp
