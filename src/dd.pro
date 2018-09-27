!win32: error("This project only supports Win32 platform!")
!versionAtLeast(QT_VERSION, 5.6.3): error("Upgrade your Qt to at least 5.6.3!")
isEmpty(ROOT): ROOT = $$PWD/..
include($$ROOT/version.pri)
include($$ROOT/optimization.pri)
TARGET = dd
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
} else {
    CONFIG *= update_translations release_translations
}
CONFIG(update_translations) {
    isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate
    system("$${lupdate} -no-obsolete $${_PRO_FILE_}")
}
CONFIG(release_translations) {
    isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease
    system("$${lrelease} -nounfinished -removeidentical $${_PRO_FILE_}")
}
QT += gui widgets avwidgets concurrent
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x051102
CONFIG *= c++11
CONFIG -= app_bundle
LIBS += -lUser32 -lDwmapi
HEADERS += \
    forms/preferencesdialog.h \
    settingsmanager.h \
    framelesswindow.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    settingsmanager.cpp \
    framelesswindow.cpp
FORMS += forms/preferencesdialog.ui
TRANSLATIONS += \
    translations/dd_en.ts \
    translations/dd_zh_CN.ts
RESOURCES += images.qrc
target.path = $$BIN_DIR
INSTALLS += target
CONFIG(static_build) {
    DEFINES += STATIC_BUILD
    RESOURCES += i18n.qrc
} else {
    translations.files = \
        $$PWD/translations/dd_en.qm \
        $$PWD/translations/dd_zh_CN.qm
    translations.path = $$BIN_DIR/translations
    qtavlibs.files = \
        $$[QT_INSTALL_BINS]/Qt*OpenGL.dll \
        $$[QT_INSTALL_BINS]/Qt*AV*.dll \
        $$[QT_INSTALL_BINS]/OpenAL32*.dll \
        $$[QT_INSTALL_BINS]/avcodec*.dll \
        $$[QT_INSTALL_BINS]/avdevice*.dll \
        $$[QT_INSTALL_BINS]/avfilter*.dll \
        $$[QT_INSTALL_BINS]/avformat*.dll \
        $$[QT_INSTALL_BINS]/avutil*.dll \
        $$[QT_INSTALL_BINS]/swresample*.dll \
        $$[QT_INSTALL_BINS]/swscale*.dll
    qtavlibs.path = $$BIN_DIR
    INSTALLS += translations qtavlibs
}
