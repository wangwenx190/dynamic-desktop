TARGET = ddskineditor
TEMPLATE = app
include(../common.pri)
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Skin Editor"
QT *= widgets
HEADERS *= forms/mainwindow.h
SOURCES *= \
    main.cpp \
    forms/mainwindow.cpp
FORMS *= forms/mainwindow.ui
include(../deploy.pri)
