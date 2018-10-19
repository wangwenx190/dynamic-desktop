include(../common.pri)
QMAKE_TARGET_PRODUCT = IPC
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Inter-Process Communication Module
TARGET = ipc
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = lib
DEFINES *= BUILD_SHARED_LIBRARY_DD
QT *= remoteobjects
QT -= gui
CONFIG *= dll
REPC_SOURCE = ipc.rep
REPC_REPLICA = ipc.rep
HEADERS += \
    $$PWD/../dd_dll_global.h \
    ipcserver.h \
    ipcclient.h \
    ipccore.h
SOURCES += \
    ipcserver.cpp \
    ipcclient.cpp \
    ipccore.cpp
target.path = $${BIN_DIR}
INSTALLS *= target
include(../deploy.pri)
