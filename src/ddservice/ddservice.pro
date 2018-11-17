TARGET = ddservice
CONFIG -= qt
CONFIG *= console
TEMPLATE = app
include(../common.pri)
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Auto Start Service"
LIBS *= -lAdvapi32
include(../ddutils/ddutils.pri)
SOURCES += main.cpp
