isEmpty(ROOT): ROOT = $$PWD/../..
include($$ROOT/version.pri)
QMAKE_TARGET_PRODUCT = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = A tool that make your desktop alive
RC_ICONS = images/bee.ico
include($$ROOT/optimization.pri)
TARGET = dd
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= gui widgets av avwidgets concurrent
qtHaveModule(winextras) {
    QT *= winextras
    DEFINES *= QT_HAS_WINEXTRAS
}
TEMPLATE = app
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11 c++14
LIBS *= -lDwmapi
include(../3rdparty/qtservice/qtservice.pri)
HEADERS += \
    forms/preferencesdialog.h \
    settingsmanager.h \
    framelesswindow.h \
    forms/aboutdialog.h \
    skinmanager.h \
    utils.h \
    mainwindow.h \
    slider.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    settingsmanager.cpp \
    framelesswindow.cpp \
    forms/aboutdialog.cpp \
    skinmanager.cpp \
    utils.cpp \
    mainwindow.cpp \
    slider.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui
TRANSLATIONS += \
    translations/dd_en.ts \
    translations/dd_zh_CN.ts
RESOURCES += images.qrc
isEmpty(qttools_dir): qttools_dir = $$[QT_INSTALL_BINS]
isEmpty(lupdate): lupdate = $${qttools_dir}/lupdate.exe
isEmpty(lrelease): lrelease = $${qttools_dir}/lrelease.exe
lrelease_params = -nounfinished -removeidentical
CONFIG(small): lrelease_params = $${lrelease_params} -compress
exists("$${lupdate}") {
    system("$${lupdate} -no-obsolete $${_PRO_FILE_}")
    system("$${lrelease} $${lrelease_params} $${_PRO_FILE_}")
}
target.path = $$BIN_DIR
INSTALLS *= target
CONFIG(static_dd) {
    DEFINES *= BUILD_DD_STATIC
    RESOURCES += \
        i18n.qrc \
        skins.qrc
} else {
    skins.path = $$BIN_DIR/skins
    skins.files = $$PWD/stylesheets/*.css
    translations.path = $$BIN_DIR/translations
    translations.files = $$PWD/translations/dd_*.qm
    exists("$${lupdate}") {
        translations.commands += $$quote(\"$${lupdate}\" -no-obsolete \"$${_PRO_FILE_}\")
        translations.commands += $$quote(\"$${lrelease}\" $${lrelease_params} \"$${_PRO_FILE_}\")
        translations.commands = $$join(translations.commands, $$escape_expand(\\n\\t))
    }
    isEmpty(ffmpeg_dir): ffmpeg_dir = $$[QT_INSTALL_BINS]
    libs.path = $$BIN_DIR
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
    INSTALLS *= libs translations skins
}
CONFIG(upx) {
    isEmpty(upx_dir): upx_dir = $$[QT_INSTALL_BINS]
    isEmpty(upx): upx = $${upx_dir}/upx.exe
    exists("$${upx}") {
        upx.path = $$BIN_DIR
        upx.commands += $$quote(\"$${upx}\" --force --ultra-brute \"$${BIN_DIR}/*.exe\")
        upx.commands += $$quote(\"$${upx}\" --force --ultra-brute \"$${BIN_DIR}/*.dll\")
        upx.commands = $$join(upx.commands, $$escape_expand(\\n\\t))
        INSTALLS *= upx
    }
}
