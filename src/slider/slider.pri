sl_lib_name = slider
sl_lib_file = slider
sl_lib_dir = release
CONFIG(debug, debug|release) {
    sl_lib_file = $$join(sl_lib_file,,,d)
    sl_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${sl_lib_name}/$${sl_lib_dir} -l$${sl_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${sl_lib_name}
DEPENDPATH += $$PWD/../$${sl_lib_name}
