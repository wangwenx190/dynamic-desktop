!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG -= ordered
qtavlib.file = src/3rdparty/qtav/qtav-lite.pro
utilslib.file = src/ddutils/ddutils.pro
service.file = src/ddservice/ddservice.pro
service.depends *= utilslib
main.file = src/ddmain/ddmain.pro
main.depends *= \
    qtavlib \
    utilslib
SUBDIRS *= \
    qtavlib \
    utilslib \
    main \
    service
