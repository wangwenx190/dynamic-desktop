ROOT = $$PWD/..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
contains(QT_ARCH, x86_64): BIN_DIR = $$join(BIN_DIR,,,64)
exists("$${ROOT}/version_ci.pri"): include($${ROOT}/version_ci.pri)
isEmpty(DD_MAJOR_VERSION): DD_MAJOR_VERSION = 1
isEmpty(DD_MINOR_VERSION): DD_MINOR_VERSION = 0
isEmpty(DD_PATCH_VERSION): DD_PATCH_VERSION = 0
isEmpty(DD_BUILD_VERSION): DD_BUILD_VERSION = 0
DESTDIR = $${BIN_DIR}
CONFIG *= c++1z
CONFIG(qt) {
    DEFINES *= \
        QT_DEPRECATED_WARNINGS \
        QT_DISABLE_DEPRECATED_BEFORE=0x050603
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
CONFIG -= app_bundle
isEmpty(VERSION): VERSION = $${DD_MAJOR_VERSION}.$${DD_MINOR_VERSION}.$${DD_PATCH_VERSION}.$${DD_BUILD_VERSION}
QMAKE_TARGET_COMPANY = "wangwenx190"
QMAKE_TARGET_COPYRIGHT = "GNU General Public License version 3.0"
QMAKE_TARGET_PRODUCT = "Dynamic Desktop"
