isEmpty(ROOT): ROOT = $$PWD/..
exists($$ROOT/version_ci.pri) {
    include($$ROOT/version_ci.pri)
} else {
    DD_VERSION     = 1.0.0
    DD_COMMIT_ID   = Not available
    DD_COMMIT_TIME = Not available
}
DD_SOURCE_CODE    = https://github.com/wangwenx190/dynamic-desktop
DD_ISSUES         = https://github.com/wangwenx190/dynamic-desktop/issues
DD_LATEST_RELEASE = https://github.com/wangwenx190/dynamic-desktop/releases/latest
VERSION = $${DD_VERSION}
DEFINES += \
    DD_VERSION=\\\"$${VERSION}\\\" \
    DD_SOURCE_CODE=\\\"$${DD_SOURCE_CODE}\\\" \
    DD_ISSUES=\\\"$${DD_ISSUES}\\\" \
    DD_LATEST_RELEASE=\\\"$${DD_LATEST_RELEASE}\\\" \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
win32 {
    QMAKE_TARGET_PRODUCT     = Dynamic Desktop
    QMAKE_TARGET_DESCRIPTION = A tool that make your desktop alive.
    QMAKE_TARGET_COMPANY     = wangwenx190
    QMAKE_TARGET_COPYRIGHT   = GPLv3
    RC_ICONS                 = $$ROOT/src/resources/icon.ico
}
