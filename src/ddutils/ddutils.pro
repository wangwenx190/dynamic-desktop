TARGET = ddutils
CONFIG -= qt
CONFIG *= dll
TEMPLATE = lib
include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Win32 Utils Module"
DEFINES *= BUILD_SHARED_LIBRARY_DD
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
SOURCES += win32utils.cpp
HEADERS += \
    win32utils.h \
    dll_global.h
