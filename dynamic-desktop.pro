!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG *= ordered
qtHaveModule(network) {
    SUBDIRS += \
        src/qsimpleupdater \
        src/udt
    DEFINES *= QT_HAS_NETWORK
}
SUBDIRS += \
    src/qtservice \
    src/wallpaper \
    src/utils \
    src/framelesswindow \
    src/skinsmanager \
    src/settingsmanager \
    src/svc \
    src/slider \
    src/dd
