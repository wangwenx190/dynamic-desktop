utl_lib_name = utils
utl_lib_file = utils
utl_lib_dir = release
CONFIG(debug, debug|release) {
    utl_lib_file = $$join(utl_lib_file,,,d)
    utl_lib_dir = debug
}
#LIBS += -L$$OUT_PWD/../$${utl_lib_name}/$${utl_lib_dir} -l$${utl_lib_file}$${DD_MAJOR_VERSION}
LIBS *= -L$${DESTDIR} -l$${utl_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${utl_lib_name}
DEPENDPATH += $$PWD/../$${utl_lib_name}
