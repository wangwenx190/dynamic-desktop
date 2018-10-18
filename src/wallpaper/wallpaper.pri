wpp_lib_name = wallpaper
wpp_lib_file = wallpaper
wpp_lib_dir = release
CONFIG(debug, debug|release) {
    wpp_lib_file = $$join(wpp_lib_file,,,d)
    wpp_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${wpp_lib_name}/$${wpp_lib_dir} -l$${wpp_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += $$PWD/../3rdparty/$${wpp_lib_name}
DEPENDPATH += $$PWD/../3rdparty/$${wpp_lib_name}
