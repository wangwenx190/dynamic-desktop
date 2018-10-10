isEmpty(ROOT): ROOT = $$PWD/../..
include($$ROOT/version.pri)
include($$ROOT/optimization.pri)
TARGET = ddsvc
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11
CONFIG += console
LIBS += -lUser32
HEADERS += \
    qtservice.h \
    qtservice_p.h
SOURCES += \
    main.cpp \
    qtservice.cpp \
    qtservice_win.cpp
target.path = $$BIN_DIR
INSTALLS += target
!CONFIG(static_dd) {
    libs.path = $$BIN_DIR
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --no-patchqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS += libs
}
