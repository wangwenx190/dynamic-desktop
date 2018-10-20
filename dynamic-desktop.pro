!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG *= ordered
SUBDIRS += \
    src/qsimpleupdater \
    src/qtniceframelesswindow \
    src/qtservice \
    src/wallpaper \
    src/utils \
    src/slider \
    src/skinsmanager \
    src/settingsmanager \
    src/ipc \
    src/core \
    src/launcher
