qs_lib_name = qtsingleapplication
qs_lib_file = qtsingleapplication
qs_lib_dir = release
CONFIG(debug, debug|release) {
    qs_lib_file = $$join(qs_lib_file,,,d)
    qs_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${qs_lib_name}/$${qs_lib_dir} -l$${qs_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${qs_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${qs_lib_name}
