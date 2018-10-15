include(../common.pri)
QMAKE_TARGET_PRODUCT = Launcher
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Launcher
RC_ICONS = images/bee.ico
TARGET = launcher
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= gui widgets av avwidgets concurrent
CONFIG *= c++14
qtHaveModule(winextras) {
    QT *= winextras
    DEFINES *= QT_HAS_WINEXTRAS
}
TEMPLATE = app
include(../qtniceframelesswindow/qtniceframelesswindow.pri)
include(../utils/utils.pri)
include(../settingsmanager/settingsmanager.pri)
include(../skinsmanager/skinsmanager.pri)
include(../slider/slider.pri)
include(../wallpaper/wallpaper.pri)
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
    translations/dd_en.ts \
    translations/dd_zh_CN.ts
RESOURCES += images.qrc
isEmpty(lupdate): lupdate = $${qttools_dir}/lupdate.exe
isEmpty(lrelease): lrelease = $${qttools_dir}/lrelease.exe
lrelease_params = -nounfinished -removeidentical
CONFIG(small): lrelease_params = $${lrelease_params} -compress
exists("$${lupdate}") {
    system("$${lupdate} -no-obsolete $${_PRO_FILE_}")
    system("$${lrelease} $${lrelease_params} $${_PRO_FILE_}")
}
target.path = $${BIN_DIR}
INSTALLS *= target
CONFIG(static_dd) {
    DEFINES *= BUILD_DD_STATIC
    RESOURCES *= \
        skins.qrc \
        i18n.qrc
} else {
    skins.path = $${BIN_DIR}/skins
    skins.files = stylesheets/*.css
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
    translations.path = $${BIN_DIR}/translations
    translations.files = translations/*.qm
    exists("$${lupdate}") {
        translations.commands += $$quote(\"$${lupdate}\" -no-obsolete \"$${_PRO_FILE_}\")
        translations.commands += $$quote(\"$${lrelease}\" $${lrelease_params} \"$${_PRO_FILE_}\")
        translations.commands = $$join(translations.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= \
        libs \
        skins \
        translations
}
include(../upx.pri)
