isEmpty(ROOT): ROOT = $$PWD/../..
exists($$ROOT/version_ci.pri) {
    include($$ROOT/version_ci.pri)
} else {
    DD_VERSION         = 1.0.0
    DD_COMMIT_ID       = -
    DD_COMMIT_TIME     = -
    FFMPEG_VERSION_STR = unknown
}
VERSION = $${DD_VERSION}
DEFINES += \
    DD_MUTEX=\\\"wangwenx190.DynamicDesktop.1000.AppMutex\\\" \
    DD_VERSION=\\\"$${VERSION}\\\" \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\" \
    FFMPEG_VERSION_STR=\\\"$${FFMPEG_VERSION_STR}\\\"
QMAKE_TARGET_COMPANY   = wangwenx190
QMAKE_TARGET_COPYRIGHT = GPLv3
