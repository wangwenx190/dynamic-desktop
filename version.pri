VERSION = 1.0.0
DEFINES += DD_VERSION=\\\"$${VERSION}\\\"
win32 {
    QMAKE_TARGET_PRODUCT     = Dynamic Desktop
    QMAKE_TARGET_DESCRIPTION = A tool that make your desktop alive.
    QMAKE_TARGET_COMPANY     = wangwenx190
    QMAKE_TARGET_COPYRIGHT   = GPLv3
    RC_ICONS                 = src/resources/icon.ico
}
