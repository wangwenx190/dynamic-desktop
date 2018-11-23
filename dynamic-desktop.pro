!win32: error("This project only supports Win32 platform!")
# Qt didn't have versionAtMost function until 5.9
greaterThan(QT_MAJOR_VERSION, 4): lessThan(QT_MINOR_VERSION, 6): error("Your Qt version is $${QT_VERSION}, however 5.6.3 is the oldest supported version.")
TEMPLATE = subdirs
CONFIG -= ordered
qtav.file = src/3rdparty/qtav/qtav-lite.pro
utils.file = src/ddutils/ddutils.pro
service.file = src/ddservice/ddservice.pro
service.depends *= utils
skineditor.file = src/ddskineditor/ddskineditor.pro
main.file = src/ddmain/ddmain.pro
main.depends *= \
    qtav \
    utils
SUBDIRS *= \
    qtav \
    utils \
    service \
    skineditor \
    main
