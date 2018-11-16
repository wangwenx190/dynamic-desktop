!win32: error("This project only supports Win32 platform!")
TEMPLATE = subdirs
CONFIG -= ordered
utils.file = src/ddutils/ddutils.pro
service.file = src/ddservice/ddservice.pro
service.depends *= utils
main.file = src/ddmain/ddmain.pro
main.depends *= utils
SUBDIRS *= \
    utils \
    service \
    main
