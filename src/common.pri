isEmpty(ROOT): ROOT = $$PWD/..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
contains(QT_ARCH, x86_64): BIN_DIR = $$join(BIN_DIR,,,64)
exists("$${ROOT}/version_ci.pri"): include($${ROOT}/version_ci.pri)
isEmpty(DD_MAJOR_VERSION): DD_MAJOR_VERSION = 1
isEmpty(DD_MINOR_VERSION): DD_MINOR_VERSION = 0
isEmpty(DD_PATCH_VERSION): DD_PATCH_VERSION = 0
isEmpty(DD_BUILD_VERSION): DD_BUILD_VERSION = 0
isEmpty(DD_COMMIT_ID): DD_COMMIT_ID = -
isEmpty(DD_COMMIT_TIME): DD_COMMIT_TIME = -
DEFINES *= \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
DESTDIR = $${BIN_DIR}
CONFIG *= c++1z
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
CONFIG(enable_small) {
    CONFIG -= enable_optimize
    CONFIG *= enable_ltcg
} else {
    CONFIG *= enable_optimize
}
include($$PWD/optimize.pri)
CONFIG -= app_bundle
CONFIG(enable_lite_build) {
    message("Building lite version of Dynamic Desktop.")
    DEFINES *= BUILD_DD_LITE
    CONFIG *= \
        no_tooltip \
        no_css \
        no_mime_type \
        no_win_extras \
        no_svg \
        no_drag_drop \
        no_menu \
        no_wheel_event \
        no_translations \
        no_commandline_parser
} else {
    isEmpty(VERSION): VERSION = $${DD_MAJOR_VERSION}.$${DD_MINOR_VERSION}.$${DD_PATCH_VERSION}.$${DD_BUILD_VERSION}
    QMAKE_TARGET_COMPANY = "wangwenx190"
    QMAKE_TARGET_COPYRIGHT = "GNU General Public License version 3.0"
    QMAKE_TARGET_PRODUCT = "Dynamic Desktop"
}
CONFIG(static, static|shared) {
    message("Building static version of Dynamic Desktop.")
    DEFINES *= BUILD_DD_STATIC
}
