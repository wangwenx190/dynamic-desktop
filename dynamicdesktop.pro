!win32: error(This project only supports Win32 platform!)
!versionAtLeast(QT_VERSION, 5.6.3): error(Upgrade your Qt to at least 5.6.3!)
include(version.pri)
include(optimization.pri)
TARGET = dd
DESTDIR = $$PWD/build/bin
OBJECTS_DIR = $$DESTDIR/../obj
MOC_DIR = $$DESTDIR/../moc
RCC_DIR = $$DESTDIR/../rcc
UI_DIR = $$DESTDIR/../ui
contains(QT_ARCH, x86_64) {
    DESTDIR = $$join(DESTDIR,,,64)
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
TRANSLATIONS += translations/dd_zh_CN.ts
RESOURCES += resources.qrc
target.path = $$DESTDIR
INSTALLS += target
