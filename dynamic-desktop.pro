!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG -= ordered
qtavlib.file = src/3rdparty/qtav/qtav-lite.pro
utilslib.file = src/ddutils/ddutils.pro
service.file = src/ddservice/ddservice.pro
service.depends *= utilslib
mainlib.file = src/ddmain/ddmain.pro
mainlib.depends *= \
    qtavlib \
    utilslib
launcher.file = src/ddlauncher/ddlauncher.pro
launcher.depends *= utilslib
SUBDIRS *= \
    qtavlib \
    utilslib \
    mainlib \
    service
CONFIG(enable_launcher): SUBDIRS *= launcher
