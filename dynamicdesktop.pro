!win32: error("This project only supports Win32 platform!")
!versionAtLeast(QT_VERSION, 5.6.3): error("Upgrade your Qt to at least 5.6.3!")
include(version.pri)
include(optimization.pri)
TARGET = dd
BIN_DIR = $$PWD/build/bin
OBJECTS_DIR = $$BIN_DIR/../obj
MOC_DIR = $$BIN_DIR/../moc
RCC_DIR = $$BIN_DIR/../rcc
UI_DIR = $$BIN_DIR/../ui
contains(QT_ARCH, x86_64) {
    BIN_DIR = $$join(BIN_DIR,,,64)
    TARGET = $$join(TARGET,,,64)
}
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
QT += gui widgets avwidgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x051102
CONFIG *= c++11
CONFIG -= app_bundle
LIBS += -lUser32
SOURCES += main.cpp forms/preferencesdialog.cpp settingsmanager.cpp
HEADERS += forms/preferencesdialog.h settingsmanager.h
FORMS += forms/preferencesdialog.ui
TRANSLATIONS += translations/dd_en.ts translations/dd_zh_CN.ts
RESOURCES += resources.qrc
target.path = $$BIN_DIR
translations.files = $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm $$PWD/translations/dd_en.qm $$PWD/translations/dd_zh_CN.qm
translations.path = $$BIN_DIR/translations
qtavlibs.files = $$[QT_INSTALL_BINS]/Qt5OpenGL.dll $$[QT_INSTALL_BINS]/QtAV1.dll $$[QT_INSTALL_BINS]/QtAVWidgets1.dll
qtavlibs.path = $$BIN_DIR
INSTALLS += target translations qtavlibs
