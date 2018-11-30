isEmpty(ROOT): ROOT = $$PWD/..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
isEmpty(LIB_DIR): LIB_DIR = $${BUILD_DIR}/lib
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    LIB_DIR = $$join(LIB_DIR,,,64)
}
exists("$${ROOT}/version_ci.pri"): include($${ROOT}/version_ci.pri)
isEmpty(DD_MAJOR_VERSION): DD_MAJOR_VERSION = 1
isEmpty(DD_MINOR_VERSION): DD_MINOR_VERSION = 0
isEmpty(DD_PATCH_VERSION): DD_PATCH_VERSION = 0
isEmpty(DD_BUILD_VERSION): DD_BUILD_VERSION = 0
contains(TEMPLATE, app): DESTDIR = $${BIN_DIR}
else:contains(TEMPLATE, lib): DESTDIR = $${LIB_DIR}
CONFIG(dll): DLLDESTDIR = $${BIN_DIR}
CONFIG *= c++1z
CONFIG(qt) {
    DEFINES *= \
        QT_DEPRECATED_WARNINGS \
        QT_DISABLE_DEPRECATED_BEFORE=0x050603
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
CONFIG -= app_bundle
CONFIG(static, static|shared):contains(TEMPLATE, lib): CONFIG *= dd_no_ver_info
else: CONFIG -= dd_no_ver_info
!CONFIG(dd_no_ver_info) {
    contains(TEMPLATE, app): DEFINES *= DD_EMBED_ICON
    RC_FILE *= $$PWD/dd_version.rc
}
