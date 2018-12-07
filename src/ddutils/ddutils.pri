ddu_lib_name = DDUtils
ddu_lib_file = DDUtils
ddu_lib_dir = release
CONFIG(debug, debug|release) {
    ddu_lib_file = $$join(ddu_lib_file,,,d)
    ddu_lib_dir = debug
}
#LIBS *= -L$$OUT_PWD/../$${ddu_lib_name}/$${ddu_lib_dir} -l$${ddu_lib_file}$${DD_MAJOR_VERSION}
isEmpty(DD_MAJOR_VERSION): DD_MAJOR_VERSION = 1
LIBS *= -L$${LIB_DIR} -l$${ddu_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH *= $$PWD/../$${ddu_lib_name}
DEPENDPATH *= $$PWD/../$${ddu_lib_name}
