fw_lib_name = framelesswindow
fw_lib_file = framelesswindow
fw_lib_dir = release
contains(QT_ARCH, x86_64): fw_lib_file = $$join(fw_lib_file,,,64)
CONFIG(debug, debug|release) {
    fw_lib_file = $$join(fw_lib_file,,,d)
    fw_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${fw_lib_name}/$${fw_lib_dir} -l$${fw_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += \
    $$PWD/../$${fw_lib_name} \
    $$PWD/../3rdparty/$${fw_lib_name}
DEPENDPATH += \
    $$PWD/../$${fw_lib_name} \
    $$PWD/../3rdparty/$${fw_lib_name}
