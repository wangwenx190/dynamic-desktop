!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG *= ordered
qtHaveModule(network) {
    SUBDIRS += \
        src/qsimpleupdater \
        src/updater
    DEFINES *= QT_HAS_NETWORK
}
SUBDIRS += \
    src/qtservice \
    src/wallpaper \
    src/utils \
    src/qtniceframelesswindow \
    src/skinsmanager \
    src/settingsmanager \
    src/service \
    src/slider \
    src/launcher
