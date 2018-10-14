ut_lib_name = utils
ut_lib_file = utils
ut_lib_dir = release
contains(QT_ARCH, x86_64): ut_lib_file = $$join(ut_lib_file,,,64)
CONFIG(debug, debug|release) {
    ut_lib_file = $$join(ut_lib_file,,,d)
    ut_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${ut_lib_name}/$${ut_lib_dir} -l$${ut_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${ut_lib_name}
DEPENDPATH += $$PWD/../$${ut_lib_name}
