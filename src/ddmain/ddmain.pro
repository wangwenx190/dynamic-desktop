isEmpty(ROOT): ROOT = $$PWD/../..
isEmpty(BUILD_DIR): BUILD_DIR = $${ROOT}/build
isEmpty(BIN_DIR): BIN_DIR = $${BUILD_DIR}/bin
contains(QT_ARCH, x86_64): BIN_DIR = $$join(BIN_DIR,,,64)
isEmpty(DD_COMMIT_ID): DD_COMMIT_ID = -
isEmpty(DD_COMMIT_TIME): DD_COMMIT_TIME = -
isEmpty(VERSION): VERSION = 1.0.0
DEFINES *= \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
QMAKE_TARGET_COMPANY = "wangwenx190"
QMAKE_TARGET_COPYRIGHT = "GNU General Public License version 3.0"
QMAKE_TARGET_PRODUCT = "Dynamic Desktop"
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
RC_ICONS = ../resources/icons/color_palette.ico
CONFIG *= c++1z
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
DESTDIR = $${BIN_DIR}
TARGET = ddmain
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= \
    widgets \
    winextras \
    avwidgets
LIBS *= \
    -lUser32 \
    -lDwmapi
TEMPLATE = app
include(../ddutils/ddutils.pri)
HEADERS += \
    forms/preferencesdialog.h \
    forms/aboutdialog.h \
    forms/traymenu.h \
    playerwindow.h \
    qtniceframelesswindow.h \
    settingsmanager.h \
    skinsmanager.h \
    slider.h \
    utils.h \
    wallpaper.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    forms/aboutdialog.cpp \
    forms/traymenu.cpp \
    playerwindow.cpp \
    qtniceframelesswindow.cpp \
    settingsmanager.cpp \
    skinsmanager.cpp \
    slider.cpp \
    utils.cpp \
    wallpaper.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui \
    forms/traymenu.ui
TRANSLATIONS += ../resources/translations/dd_zh_CN.ts
RESOURCES += ddmain.qrc
isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate.exe
exists("$${lupdate}"): system("$${lupdate} -no-obsolete -locations none -no-ui-lines $${_PRO_FILE_}")
isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
exists("$${lrelease}"): system("$${lrelease} -compress -nounfinished -removeidentical $${_PRO_FILE_}")
libs.path = $${BIN_DIR}
isEmpty(ffmpeg_dir): ffmpeg_dir = $$[QT_INSTALL_BINS]
libs.files = \
    $${ffmpeg_dir}/avcodec-*.dll \
    $${ffmpeg_dir}/avdevice-*.dll \
    $${ffmpeg_dir}/avfilter-*.dll \
    $${ffmpeg_dir}/avformat-*.dll \
    $${ffmpeg_dir}/avresample-*.dll \
    $${ffmpeg_dir}/avutil-*.dll \
    $${ffmpeg_dir}/ass.dll \
    $${ffmpeg_dir}/libass.dll \
    $${ffmpeg_dir}/OpenAL32*.dll \
    $${ffmpeg_dir}/postproc-*.dll \
    $${ffmpeg_dir}/swresample-*.dll \
    $${ffmpeg_dir}/swscale-*.dll
CONFIG(shared, static|shared):CONFIG(debug, debug|release) {
    libs.files *= \
        $$[QT_INSTALL_BINS]/QtAVd?.dll \
        $$[QT_INSTALL_BINS]/QtAVWidgetsd?.dll
}
CONFIG(shared, static|shared):CONFIG(release, debug|release) {
    libs.files *= \
        $$[QT_INSTALL_BINS]/QtAV?.dll \
        $$[QT_INSTALL_BINS]/QtAVWidgets?.dll
}
CONFIG(shared, static|shared) {
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw -opengl --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
}
INSTALLS *= libs
