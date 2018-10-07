!win32: error("This project only supports Win32 platform!")
isEmpty(ROOT): ROOT = $$PWD/..
include($$ROOT/version.pri)
include($$ROOT/optimization.pri)
TARGET = dd
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT += gui widgets av avwidgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11
CONFIG -= app_bundle
LIBS += -lUser32 -lDwmapi
HEADERS += \
    forms/preferencesdialog.h \
    settingsmanager.h \
    framelesswindow.h \
    forms/aboutdialog.h \
    skinmanager.h \
    utils.h \
    mainwindow.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    settingsmanager.cpp \
    framelesswindow.cpp \
    forms/aboutdialog.cpp \
    skinmanager.cpp \
    utils.cpp \
    mainwindow.cpp
FORMS += forms/preferencesdialog.ui \
    forms/aboutdialog.ui
TRANSLATIONS += \
    translations/dd_en.ts \
    translations/dd_zh_CN.ts
RESOURCES += images.qrc
isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate.exe
isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
exists("$${lupdate}") {
    system("$${lupdate} -no-obsolete $${_PRO_FILE_}")
    system("$${lrelease} -nounfinished -removeidentical $${_PRO_FILE_}")
}
target.path = $$BIN_DIR
INSTALLS += target
CONFIG(static_dd) {
    DEFINES += BUILD_DD_STATIC
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
        translations.commands += $$quote(\"$${lrelease}\" -nounfinished -removeidentical \"$${_PRO_FILE_}\")
        translations.commands = $$join(translations.commands, $$escape_expand(\\n\\t))
    }
    libs.path = $$BIN_DIR
    libs.files = \
        $$[QT_INSTALL_BINS]/QtAV*.dll \
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
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --force --no-translations --no-system-d3d-compiler --compiler-runtime --no-angle --no-opengl-sw -opengl --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS += libs translations skins
}
