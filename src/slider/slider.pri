sld_lib_name = slider
sld_lib_file = slider
sld_lib_dir = release
CONFIG(debug, debug|release) {
    sld_lib_file = $$join(sld_lib_file,,,d)
    sld_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${sld_lib_name}/$${sld_lib_dir} -l$${sld_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${sld_lib_name}
DEPENDPATH += $$PWD/../$${sld_lib_name}
