qsu_lib_name = qsimpleupdater
qsu_lib_file = qsimpleupdater
qsu_lib_dir = release
contains(QT_ARCH, x86_64): qsu_lib_file = $$join(qsu_lib_file,,,64)
CONFIG(debug, debug|release) {
    qsu_lib_file = $$join(qsu_lib_file,,,d)
    qsu_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${qsu_lib_name}/$${qsu_lib_dir} -l$${qsu_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += \
    $$PWD/../$${qsu_lib_name} \
    $$PWD/../3rdparty/$${qsu_lib_name}/include
DEPENDPATH += \
    $$PWD/../$${qsu_lib_name} \
    $$PWD/../3rdparty/$${qsu_lib_name}/include
