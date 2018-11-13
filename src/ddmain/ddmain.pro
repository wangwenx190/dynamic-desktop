TARGET = ddmain
include(../common.pri)
isEmpty(DD_COMMIT_ID): DD_COMMIT_ID = -
isEmpty(DD_COMMIT_TIME): DD_COMMIT_TIME = -
DEFINES *= \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
CONFIG(static, static|shared) {
    message("Building static version of Dynamic Desktop.")
    DEFINES *= BUILD_DD_STATIC
}
QT *= \
    widgets \
    network \
    opengl \
    avwidgets
!qtHaveModule(svg) {
    DEFINES *= DD_NO_SVG
    QT -= svg
    RESOURCES *= images_png.qrc
} else {
    QT *= svg
    RESOURCES *= images_svg.qrc
}
!qtHaveModule(winextras) {
    DEFINES *= DD_NO_WIN_EXTRAS
} else {
    QT *= winextras
}
!CONTAINS(QT_CONFIG, draganddrop): DEFINES *= DD_NO_DRAG_DROP
!CONTAINS(QT_CONFIG, mimetype): DEFINES *= DD_NO_MIME_TYPE
!CONTAINS(QT_CONFIG, tooltip): DEFINES *= DD_NO_TOOLTIP
!CONTAINS(QT_CONFIG, cssparser) {
    DEFINES *= DD_NO_CSS
} else {
    RESOURCES *= skins.qrc
    HEADERS *= skinsmanager.h
    SOURCES *= skinsmanager.cpp
}
!CONTAINS(QT_CONFIG, menu) {
    DEFINES *= DD_NO_MENU
} else {
    FORMS *= forms/traymenu.ui
    HEADERS *= forms/traymenu.h
    SOURCES *= forms/traymenu.cpp
}
!CONTAINS(QT_CONFIG, wheelevent): DEFINES *= DD_NO_WHEEL_EVENT
!CONTAINS(QT_CONFIG, translation) {
    DEFINES *= \
        DD_NO_TRANSLATIONS \
        DD_TR=QStringLiteral \
        DD_OBJ_TR=QStringLiteral \
        DD_APP_TR=QStringLiteral
} else {
    DEFINES *= \
        DD_TR=tr \
        DD_OBJ_TR=QObject::tr \
        DD_APP_TR=QtSingleApplication::translate
    TRANSLATIONS *= ../resources/translations/dd_zh_CN.ts
    CONFIG(update_translations) {
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
    RESOURCES *= translations.qrc
}
!CONTAINS(QT_CONFIG, commandlineparser): DEFINES *= DD_NO_COMMANDLINE_PARSER
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
include(../deploy.pri)
