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
QT += gui widgets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x051102
CONFIG *= c++11
CONFIG -= app_bundle
LIBS += -lUser32 -lKernel32 -lShell32
SOURCES += \
    main.cpp \
    wallpaperwidget.cpp \
    forms/preferencesdialog.cpp \
    utils.cpp
HEADERS += \
    wallpaperwidget.h \
    forms/preferencesdialog.h \
    utils.h
FORMS += forms/preferencesdialog.ui
TRANSLATIONS += translations/dd_zh_CN.ts
target.path = $$DESTDIR
INSTALLS += target
