qs_lib_name = qtservice
qs_lib_file = qtservice
qs_lib_dir = release
contains(QT_ARCH, x86_64): qs_lib_file = $$join(qs_lib_file,,,64)
CONFIG(debug, debug|release) {
    qs_lib_file = $$join(qs_lib_file,,,d)
    qs_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${qs_lib_name}/$${qs_lib_dir} -l$${qs_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += \
    $$PWD/../$${qs_lib_name} \
    $$PWD/../3rdparty/$${qs_lib_name}
DEPENDPATH += \
    $$PWD/../$${qs_lib_name} \
    $$PWD/../3rdparty/$${qs_lib_name}
