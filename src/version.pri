exists($${ROOT}/version_ci.pri) {
    include($${ROOT}/version_ci.pri)
} else {
    DD_MAJOR_VERSION = 1
    DD_MINOR_VERSION = 0
    DD_PATCH_VERSION = 0
    DD_BUILD_VERSION = 0
    DD_COMMIT_ID = -
    DD_COMMIT_TIME = -
    FFMPEG_VERSION_STR = Unknown
}
VERSION = $${DD_MAJOR_VERSION}.$${DD_MINOR_VERSION}.$${DD_PATCH_VERSION}.$${DD_BUILD_VERSION}
DEFINES *= \
    DD_MUTEX=\\\"wangwenx190.DynamicDesktop.1000.AppMutex\\\" \
    DD_VERSION=\\\"$${VERSION}\\\" \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\" \
    FFMPEG_VERSION_STR=\\\"$${FFMPEG_VERSION_STR}\\\"
QMAKE_TARGET_COMPANY = wangwenx190
QMAKE_TARGET_COPYRIGHT = GPLv3
