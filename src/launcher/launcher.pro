include(../common.pri)
QMAKE_TARGET_PRODUCT = Launcher
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Launcher
RC_ICONS = ../resources/icons/color_palette.ico
TARGET = launcher
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= gui widgets av avwidgets concurrent winextras
TEMPLATE = app
include(../qtniceframelesswindow/qtniceframelesswindow.pri)
include(../utils/utils.pri)
include(../settingsmanager/settingsmanager.pri)
include(../skinsmanager/skinsmanager.pri)
include(../slider/slider.pri)
include(../wallpaper/wallpaper.pri)
include(../qtservice/qtservice.pri)
LIBS *= -lUser32
HEADERS += \
    forms/preferencesdialog.h \
    forms/aboutdialog.h \
    mainwindow.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    forms/aboutdialog.cpp \
    mainwindow.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui
TRANSLATIONS += \
    ../resources/translations/launcher_en.ts \
    ../resources/translations/launcher_zh_CN.ts
RESOURCES += images.qrc
include(../translations.pri)
target.path = $${BIN_DIR}
INSTALLS *= target
CONFIG(static_dd) {
    DEFINES *= BUILD_DD_STATIC
    RESOURCES *= skins.qrc
} else {
    skins.path = $${BIN_DIR}/skins
    skins.files = ../resources/stylesheets/*.css
    libs.path = $${BIN_DIR}
    libs.files = \
        $$[QT_INSTALL_BINS]/QtAV?.dll \
        $$[QT_INSTALL_BINS]/QtAVWidgets?.dll \
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
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --compiler-runtime --angle --no-opengl-sw -concurrent -opengl --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= \
        libs \
        skins
}
licenses.path = $${BIN_DIR}/licenses
licenses.files = $${ROOT}/docs/licenses/*
INSTALLS *= licenses
include(../upx.pri)
