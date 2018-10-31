include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Win32 Utils Module"
TARGET = ddutils
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
CONFIG -= qt
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
SOURCES += win32utils.cpp
HEADERS += \
    win32utils.h \
    dll_global.h
