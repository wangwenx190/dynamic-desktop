!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG -= ordered
qtav.file = src/3rdparty/qtav/qtav-lite.pro
utils.file = src/ddutils/ddutils.pro
service.file = src/ddservice/ddservice.pro
service.depends *= utils
main.file = src/ddmain/ddmain.pro
main.depends *= \
    qtav \
    utils
SUBDIRS *= \
    qtav \
    utils \
    service \
    main
