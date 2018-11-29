TARGET = DDLauncher
CONFIG -= qt
CONFIG *= console
TEMPLATE = app
include(../common.pri)
LIBS *= \
    -lKernel32 \
    -lUser32 \
    -lShell32
SOURCES *= main.cpp
