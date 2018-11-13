TARGET = ddservice
CONFIG -= qt
include(../common.pri)
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Auto Start Service"
CONFIG *= console
LIBS *= -lAdvapi32
include(../ddutils/ddutils.pri)
TEMPLATE = app
SOURCES += main.cpp
