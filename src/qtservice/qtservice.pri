qts_lib_name = qtservice
qts_lib_file = qtservice
qts_lib_dir = release
CONFIG(debug, debug|release) {
    qts_lib_file = $$join(qts_lib_file,,,d)
    qts_lib_dir = debug
}
#LIBS += -L$$OUT_PWD/../$${qts_lib_name}/$${qts_lib_dir} -l$${qts_lib_file}$${DD_MAJOR_VERSION}
LIBS *= -L$${DESTDIR} -l$${qts_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${qts_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${qts_lib_name}
