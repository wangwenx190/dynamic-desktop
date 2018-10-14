!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG *= ordered
qtHaveModule(network) {
    SUBDIRS += src/udt
    DEFINES *= QT_HAS_NETWORK
}
SUBDIRS += \
    src/svc \
    src/dd
