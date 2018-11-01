TARGET = ddmain
include(../common.pri)
exists("$${ROOT}/.qmake.conf") {
    message("\".qmake.conf\" detected, you are building Dynamic Desktop version $${VERSION}.")
} else {
    message("qmake can\'t find \".qmake.conf\", this build is for testing only.")
}
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
RC_ICONS = ../resources/icons/color_palette.ico
QT *= \
    widgets \
    winextras \
    avwidgets
CONFIG(static, static|shared) {
    QT *= \
        svg \
        opengl
}
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
exists("$${lupdate}") {
    system("$${lupdate} -no-obsolete -locations none -no-ui-lines $${_PRO_FILE_}")
} else {
    message("qmake can\'t find \"lupdate.exe\" in \"$$[QT_INSTALL_BINS]\".")
    message("The translations may be out-dated.")
}
isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
exists("$${lrelease}") {
    system("$${lrelease} -compress -nounfinished -removeidentical $${_PRO_FILE_}")
} else {
    message("qmake can\'t find \"lrelease.exe\" in \"$$[QT_INSTALL_BINS]\".")
    message("The translations may be out-dated.")
}
libs.path = $${BIN_DIR}
isEmpty(ffmpeg_dir): ffmpeg_dir = $$[QT_INSTALL_BINS]
exists("$${ffmpeg_dir}/avcodec-*.dll") {
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
} else {
    message("qmake can\'t find FFmpeg\'s run-time libraries in \"$${ffmpeg_dir}\".")
    message("You may have to copy them manually.")
    message("You can set the \"ffmpeg_dir\" variable to let qmake copy them automatically.")
}
CONFIG(shared, static|shared) {
    exists("$$[QT_INSTALL_BINS]/QtAV*.dll") {
        CONFIG(debug, debug|release) {
            libs.files *= \
                $$[QT_INSTALL_BINS]/QtAVd?.dll \
                $$[QT_INSTALL_BINS]/QtAVWidgetsd?.dll
        } else:CONFIG(release, debug|release) {
            libs.files *= \
                $$[QT_INSTALL_BINS]/QtAV?.dll \
                $$[QT_INSTALL_BINS]/QtAVWidgets?.dll
        }
    } else {
        message("qmake can\'t find QtAV\'s run-time libraries in \"$$[QT_INSTALL_BINS]\".")
        message("You may have to copy them manually.")
        message("Currently(QtAV 1.12.0) they are QtAV[d]1.dll and QtAVWidgets[d]1.dll.")
    }
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw -opengl --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    } else {
        message("It seems that there is no \"windeployqt.exe\" in \"$$[QT_INSTALL_BINS]\".")
        message("You may have to copy Qt run-time libraries manually and don\'t forget about the plugins.")
        message("Qt5Svg.dll, Qt5OpenGL.dll and plugins\\iconengines\\qsvgicon.dll are the necessary dlls you must copy.")
        message("d3dcompiler_XX.dll, libEGL.dll, libGLESv2.dll and opengl32sw.dll may be useful as well.")
    }
}
!isEmpty(libs.files): INSTALLS *= libs
