qsu_lib_name = qsimpleupdater
qsu_lib_file = qsimpleupdater
qsu_lib_dir = release
CONFIG(debug, debug|release) {
    qsu_lib_file = $$join(qsu_lib_file,,,d)
    qsu_lib_dir = debug
}
#LIBS += -L$$OUT_PWD/../$${qsu_lib_name}/$${qsu_lib_dir} -l$${qsu_lib_file}$${DD_MAJOR_VERSION}
LIBS *= -L$${DESTDIR} -l$${qsu_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${qsu_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${qsu_lib_name}
