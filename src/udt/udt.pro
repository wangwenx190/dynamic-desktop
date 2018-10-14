isEmpty(ROOT): ROOT = $$PWD/../..
include($$ROOT/version.pri)
QMAKE_TARGET_PRODUCT = Dynamic Desktop Updater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater
RC_ICONS = images/refresh.ico
include($$ROOT/optimization.pri)
TARGET = ddudt
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11
SOURCES += main.cpp
target.path = $$BIN_DIR
INSTALLS *= target
include(../3rdparty/qsimpleupdater/QSimpleUpdater.pri)
!CONFIG(static_dd) {
    libs.path = $$BIN_DIR
    isEmpty(qttools_dir): qttools_dir = $$[QT_INSTALL_BINS]
    isEmpty(windeployqt): windeployqt = $${qttools_dir}/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --no-patchqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= libs
}
