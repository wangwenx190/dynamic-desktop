isEmpty(ROOT): ROOT = $$PWD/../..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
contains(QT_ARCH, x86_64): BIN_DIR = $$join(BIN_DIR,,,64)
isEmpty(VERSION): VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "wangwenx190"
QMAKE_TARGET_COPYRIGHT = "GNU General Public License version 3.0"
QMAKE_TARGET_PRODUCT = "Dynamic Desktop"
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop Win32 Utils Module"
DESTDIR = $${BIN_DIR}
TARGET = ddutils
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
CONFIG -= qt
DEFINES *= BUILD_SHARED_LIBRARY_DD
CONFIG *= dll c++1z
LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
SOURCES += win32utils.cpp
HEADERS += \
    win32utils.h \
    dll_global.h
