isEmpty(ROOT): ROOT = $$PWD/../..
include($$ROOT/version.pri)
QMAKE_TARGET_PRODUCT     = Dynamic Desktop Updater
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Updater
RC_ICONS                 = images/refresh.ico
include($$ROOT/optimization.pri)
TARGET = ddudt
BIN_DIR = $$ROOT/bin
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= gui widgets network
qtHaveModule(winextras) {
    QT *= winextras
    DEFINES *= QT_HAS_WINEXTRAS
}
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
CONFIG *= c++11
HEADERS += \
    forms/mainwindow.h \
    downloadmanager.h
SOURCES += \
    main.cpp \
    forms/mainwindow.cpp \
    downloadmanager.cpp
FORMS += \
    forms/mainwindow.ui
TRANSLATIONS += \
    translations/udt_en.ts \
    translations/udt_zh_CN.ts
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
    RESOURCES += i18n.qrc
} else {
    translations.path = $$BIN_DIR/translations
    translations.files = $$PWD/translations/udt_*.qm
    exists("$${lupdate}") {
        translations.commands += $$quote(\"$${lupdate}\" -no-obsolete \"$${_PRO_FILE_}\")
        translations.commands += $$quote(\"$${lrelease}\" $${lrelease_params} \"$${_PRO_FILE_}\")
        translations.commands = $$join(translations.commands, $$escape_expand(\\n\\t))
    }
    libs.path = $$BIN_DIR
    isEmpty(windeployqt): windeployqt = $$[QT_INSTALL_BINS]/windeployqt.exe
    exists("$${windeployqt}") {
        libs.commands = $$quote(\"$${windeployqt}\" --plugindir \"$${BIN_DIR}/plugins\" --no-patchqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-angle --no-opengl-sw --no-svg --list source \"$${BIN_DIR}/$${TARGET}.exe\")
        libs.commands = $$join(libs.commands, $$escape_expand(\\n\\t))
    }
    INSTALLS *= libs translations
}
