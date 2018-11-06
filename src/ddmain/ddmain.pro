TARGET = ddmain
include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
RC_ICONS = ../resources/icons/color_palette.ico
QT *= \
    widgets \
    network \
    winextras \
    avwidgets
CONFIG(static, static|shared) {
    DEFINES *= BUILD_DD_STATIC
    QT *= \
        svg \
        opengl
}
LIBS *= \
    -lUser32 \
    -lDwmapi
TEMPLATE = app
include(../ddutils/ddutils.pri)
include(../qtniceframelesswindow/qtniceframelesswindow.pri)
include(../wallpaper/wallpaper.pri)
include(../qtsingleapplication/qtsingleapplication.pri)
HEADERS += \
    forms/preferencesdialog.h \
    forms/aboutdialog.h \
    forms/traymenu.h \
    playerwindow.h \
    settingsmanager.h \
    skinsmanager.h \
    slider.h \
    utils.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    forms/aboutdialog.cpp \
    forms/traymenu.cpp \
    playerwindow.cpp \
    settingsmanager.cpp \
    skinsmanager.cpp \
    slider.cpp \
    utils.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui \
    forms/traymenu.ui
TRANSLATIONS += ../resources/translations/dd_zh_CN.ts
RESOURCES += ddmain.qrc
CONFIG(update_translations) {
    isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate.exe
    exists("$${lupdate}") {
        system("$${lupdate} -no-obsolete -locations none -no-ui-lines $${_PRO_FILE_}")
    } else {
        message("qmake can\'t find \"lupdate.exe\" in \"$$[QT_INSTALL_BINS]\".")
    }
    isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
    exists("$${lrelease}") {
        system("$${lrelease} -nounfinished -removeidentical $${_PRO_FILE_}")
    } else {
        message("qmake can\'t find \"lrelease.exe\" in \"$$[QT_INSTALL_BINS]\".")
    }
}
include(../deploy.pri)
