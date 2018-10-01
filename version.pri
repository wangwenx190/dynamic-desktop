isEmpty(ROOT): ROOT = $$PWD/..
exists($$ROOT/version_ci.pri) {
    include($$ROOT/version_ci.pri)
} else {
    DD_VERSION         = 1.0.0
    DD_COMMIT_ID       = -
    DD_COMMIT_TIME     = -
    FFMPEG_VERSION_STR = 4.0.2-git
}
VERSION = $${DD_VERSION}
DEFINES += \
    DD_VERSION=\\\"$${VERSION}\\\" \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\" \
    FFMPEG_VERSION_STR=\\\"$${FFMPEG_VERSION_STR}\\\"
win32 {
    QMAKE_TARGET_PRODUCT     = Dynamic Desktop
    QMAKE_TARGET_DESCRIPTION = A tool that make your desktop alive.
    QMAKE_TARGET_COMPANY     = wangwenx190
    QMAKE_TARGET_COPYRIGHT   = GPLv3
    RC_ICONS                 = $$ROOT/src/images/bee.ico
}
