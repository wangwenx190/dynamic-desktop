skm_lib_name = skinsmanager
skm_lib_file = skinsmanager
skm_lib_dir = release
contains(QT_ARCH, x86_64): skm_lib_file = $$join(skm_lib_file,,,64)
CONFIG(debug, debug|release) {
    skm_lib_file = $$join(skm_lib_file,,,d)
    skm_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${skm_lib_name}/$${skm_lib_dir} -l$${skm_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${skm_lib_name}
DEPENDPATH += $$PWD/../$${skm_lib_name}
