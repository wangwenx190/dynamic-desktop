include(../root.pri)
include(../version.pri)
CONFIG *= c++1z
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop
RC_ICONS = resources/icons/color_palette.ico
TARGET = ddmain
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= widgets winextras avwidgets opengl
TEMPLATE = app
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
TRANSLATIONS += resources/translations/dd_zh_CN.ts
RESOURCES += ddmain.qrc
isEmpty(lupdate): lupdate = $${qttools_dir}/lupdate.exe
isEmpty(lrelease): lrelease = $${qttools_dir}/lrelease.exe
exists("$${lupdate}"): system("$${lupdate} -no-obsolete -locations none -no-ui-lines $${_PRO_FILE_}")
exists("$${lrelease}"): system("$${lrelease} -compress -nounfinished -removeidentical $${_PRO_FILE_}")
!CONFIG(static_dd) {
    libs.path = $${BIN_DIR}
    libs.files = \
        $$[QT_INSTALL_BINS]/QtAV*.dll \
        $$[QT_INSTALL_BINS]/QtAVWidgets*.dll \
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
    isEmpty(windeployqt): windeployqt = $${qttools_dir}/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw -opengl --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= libs
}
