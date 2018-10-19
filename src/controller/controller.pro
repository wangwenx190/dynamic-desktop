include(../common.pri)
QMAKE_TARGET_PRODUCT = Controller
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Controller Module
TARGET = controller
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= widgets winextras
TEMPLATE = lib
CONFIG *= dll
DEFINES *= BUILD_INNER_SHARED_LIBRARY_DD
include(../qtniceframelesswindow/qtniceframelesswindow.pri)
include(../utils/utils.pri)
include(../settingsmanager/settingsmanager.pri)
include(../skinsmanager/skinsmanager.pri)
include(../slider/slider.pri)
include(../ipc/ipc.pri)
include(../qtservice/qtservice.pri)
LIBS *= -lUser32
HEADERS += \
    $$PWD/../dd_inner_dll_global.h \
    controller.h \
    forms/preferencesdialog.h \
    forms/aboutdialog.h \
    $$PWD/../common.h
SOURCES += \
    controller.cpp \
    forms/preferencesdialog.cpp \
    forms/aboutdialog.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui
TRANSLATIONS += \
    ../resources/translations/controller_en.ts \
    ../resources/translations/controller_zh_CN.ts
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
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --compiler-runtime --angle --no-opengl-sw -opengl --no-svg --list source \"$${BIN_DIR}/$${TARGET}$${DD_MAJOR_VERSION}.dll\")
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
