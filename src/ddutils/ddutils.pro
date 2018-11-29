TARGET = DDUtils
CONFIG -= qt
CONFIG *= dll
TEMPLATE = lib
include(../common.pri)
DEFINES *= BUILD_SHARED_LIBRARY_DD_UTILS
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
SOURCES += win32utils.cpp
HEADERS += \
    win32utils.h \
    win32utils_global.h
