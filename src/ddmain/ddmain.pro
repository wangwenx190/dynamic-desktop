TARGET = ddmain
include(../common.pri)
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
RC_ICONS = ../resources/icons/color_palette.ico
QT *= \
    widgets \
    network \
    avwidgets
CONFIG(no_svg) {
    DEFINES *= DD_NO_SVG
    QT -= svg
    RESOURCES *= images_png.qrc
} else {
    QT *= svg
    RESOURCES *= images_svg.qrc
}
CONFIG(no_win_extras) {
    DEFINES *= DD_NO_WIN_EXTRAS
} else {
    QT *= winextras
}
CONFIG(no_drag_drop): DEFINES *= DD_NO_DRAG_DROP
CONFIG(no_mime_type): DEFINES *= DD_NO_MIME_TYPE
CONFIG(no_tooltip): DEFINES *= DD_NO_TOOLTIP
CONFIG(no_css) {
    DEFINES *= DD_NO_CSS
} else {
    RESOURCES *= skins.qrc
    HEADERS *= skinsmanager.h
    SOURCES *= skinsmanager.cpp
}
CONFIG(no_menu) {
    DEFINES *= DD_NO_MENU
} else {
    FORMS *= forms/traymenu.ui
    HEADERS *= forms/traymenu.h
    SOURCES *= forms/traymenu.cpp
}
CONFIG(no_wheel_event): DEFINES *= DD_NO_WHEEL_EVENT
CONFIG(no_translations) {
    DEFINES *= \
        DD_NO_TRANSLATIONS \
        DD_TR=QStringLiteral \
        DD_OBJ_TR=QStringLiteral \
        DD_APP_TR=QStringLiteral
} else {
    RESOURCES *= translations.qrc
    DEFINES *= \
        DD_TR=tr \
        DD_OBJ_TR=QObject::tr \
        DD_APP_TR=QtSingleApplication::translate
}
CONFIG(no_commandline_parser): DEFINES *= DD_NO_COMMANDLINE_PARSER
LIBS *= \
    -lUser32 \
    -lDwmapi
TEMPLATE = app
include(../ddutils/ddutils.pri)
include(../3rdparty/qtniceframelesswindow/qtniceframelesswindow.pri)
include(../3rdparty/wallpaper/wallpaper.pri)
include(../3rdparty/qtsingleapplication/qtsingleapplication.pri)
HEADERS += \
    forms/preferencesdialog.h \
    forms/aboutdialog.h \
    playerwindow.h \
    settingsmanager.h \
    slider.h \
    utils.h
SOURCES += \
    main.cpp \
    forms/preferencesdialog.cpp \
    forms/aboutdialog.cpp \
    playerwindow.cpp \
    settingsmanager.cpp \
    slider.cpp \
    utils.cpp
FORMS += \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui
TRANSLATIONS += ../resources/translations/dd_zh_CN.ts
CONFIG(update_translations):!CONFIG(no_translations) {
    isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate.exe
    exists("$${lupdate}") {
        system("$${lupdate} -no-obsolete -locations none -no-ui-lines $${_PRO_FILE_}")
    } else {
        message("qmake can\'t find \"lupdate.exe\" in \"$$[QT_INSTALL_BINS]\".")
    }
    isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
    exists("$${lrelease}") {
        system("$${lrelease} -compress -nounfinished -removeidentical $${_PRO_FILE_}")
    } else {
        message("qmake can\'t find \"lrelease.exe\" in \"$$[QT_INSTALL_BINS]\".")
    }
}
include(../deploy.pri)
