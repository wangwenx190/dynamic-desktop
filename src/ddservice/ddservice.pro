include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Auto Start Service"
RC_ICONS = ../resources/icons/color_palette.ico
TARGET = ddservice
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
CONFIG -= qt
CONFIG *= console
LIBS *= -lAdvapi32
include(../ddutils/ddutils.pri)
TEMPLATE = app
SOURCES += main.cpp
