!win32: error(This project only supports Win32 platform!)
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
QT += gui widgets svg avwidgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x051102
CONFIG *= c++11
CONFIG -= app_bundle
LIBS += -lUser32
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp
HEADERS += \
    forms/preferencesdialog.h
FORMS += forms/preferencesdialog.ui
TRANSLATIONS += translations/dd_zh_CN.ts
target.path = $$DESTDIR
INSTALLS += target
