stm_lib_name = settingsmanager
stm_lib_file = settingsmanager
stm_lib_dir = release
contains(QT_ARCH, x86_64): stm_lib_file = $$join(stm_lib_file,,,64)
CONFIG(debug, debug|release) {
    stm_lib_file = $$join(stm_lib_file,,,d)
    stm_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${stm_lib_name}/$${stm_lib_dir} -l$${stm_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${stm_lib_name}
DEPENDPATH += $$PWD/../$${stm_lib_name}
