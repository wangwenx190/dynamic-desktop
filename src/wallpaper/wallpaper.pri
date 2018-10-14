wp_lib_name = wallpaper
wp_lib_file = wallpaper
wp_lib_dir = release
contains(QT_ARCH, x86_64): wp_lib_file = $$join(wp_lib_file,,,64)
CONFIG(debug, debug|release) {
    wp_lib_file = $$join(wp_lib_file,,,d)
    wp_lib_dir = debug
}
LIBS += -L$$OUT_PWD/../$${wp_lib_name}/$${wp_lib_dir} -l$${wp_lib_file}$${DD_MAJOR_VERSION}
INCLUDEPATH += \
    $$PWD/../$${wp_lib_name} \
    $$PWD/../3rdparty/$${wp_lib_name}
DEPENDPATH += \
    $$PWD/../$${wp_lib_name} \
    $$PWD/../3rdparty/$${wp_lib_name}
