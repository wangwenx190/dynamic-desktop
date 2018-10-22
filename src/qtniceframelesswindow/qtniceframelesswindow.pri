qnfw_lib_name = qtniceframelesswindow
qnfw_lib_file = qtniceframelesswindow
qnfw_lib_dir = release
CONFIG(debug, debug|release) {
    qnfw_lib_file = $$join(qnfw_lib_file,,,d)
    qnfw_lib_dir = debug
}
#LIBS += -L$$OUT_PWD/../$${qnfw_lib_name}/$${qnfw_lib_dir} -l$${qnfw_lib_file}$${DD_MAJOR_VERSION}
LIBS *= -L$${DESTDIR} -l$${qnfw_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${qnfw_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${qnfw_lib_name}
