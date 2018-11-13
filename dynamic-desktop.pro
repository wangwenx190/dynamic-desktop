!win32: error("This project only supports Win32 platform!")
!qtHaveModule(gui): error("qmake can\'t find \"gui\" module. Compilation aborted.")
!qtHaveModule(widgets): error("qmake can\'t find \"widgets\" module. Compilation aborted.")
!qtHaveModule(network): error("qmake can\'t find \"network\" module. Compilation aborted.")
!qtHaveModule(opengl): error("qmake can\'t find \"opengl\" module. Compilation aborted.")
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
