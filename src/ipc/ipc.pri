ipc_lib_name = ipc
ipc_lib_file = ipc
ipc_lib_dir = release
CONFIG(debug, debug|release) {
    ipc_lib_file = $$join(ipc_lib_file,,,d)
    ipc_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${ipc_lib_name}/$${ipc_lib_dir} -l$${ipc_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../$${ipc_lib_name}
DEPENDPATH += $$PWD/../$${ipc_lib_name}
