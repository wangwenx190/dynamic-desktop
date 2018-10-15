fw_lib_name = qtniceframelesswindow
fw_lib_file = qtniceframelesswindow
fw_lib_dir = release
CONFIG(debug, debug|release) {
    fw_lib_file = $$join(fw_lib_file,,,d)
    fw_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${fw_lib_name}/$${fw_lib_dir} -l$${fw_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${fw_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${fw_lib_name}
