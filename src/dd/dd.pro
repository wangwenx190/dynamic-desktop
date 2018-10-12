isEmpty(ROOT): ROOT = $$PWD/../..
include($$ROOT/version.pri)
QMAKE_TARGET_PRODUCT     = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = A tool that make your desktop alive
RC_ICONS                 = images/bee.ico
include($$ROOT/optimization.pri)
TARGET = dd
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT *= gui widgets av avwidgets
qtHaveModule(winextras) {
    QT *= winextras
    DEFINES *= QT_HAS_WINEXTRAS
}
TEMPLATE = app
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11
LIBS *= \
    -lUser32 \
    -lDwmapi \
    -lAdvAPI32
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
isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate.exe
isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
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
    libs.path = $$BIN_DIR
    libs.files = \
        $$[QT_INSTALL_BINS]/QtAV?.dll \
        $$[QT_INSTALL_BINS]/QtAVWidgets?.dll \
        $$[QT_INSTALL_BINS]/avcodec-*.dll \
        $$[QT_INSTALL_BINS]/avdevice-*.dll \
        $$[QT_INSTALL_BINS]/avfilter-*.dll \
        $$[QT_INSTALL_BINS]/avformat-*.dll \
        $$[QT_INSTALL_BINS]/avresample-*.dll \
        $$[QT_INSTALL_BINS]/avutil-*.dll \
        $$[QT_INSTALL_BINS]/ass.dll \
        $$[QT_INSTALL_BINS]/libass.dll \
        $$[QT_INSTALL_BINS]/OpenAL32*.dll \
        $$[QT_INSTALL_BINS]/postproc-*.dll \
        $$[QT_INSTALL_BINS]/swresample-*.dll \
        $$[QT_INSTALL_BINS]/swscale-*.dll
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --compiler-runtime --angle --no-opengl-sw -opengl --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= libs translations skins
}
