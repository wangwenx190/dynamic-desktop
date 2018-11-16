TARGET = ddmain
include(../common.pri)
isEmpty(DD_COMMIT_ID): DD_COMMIT_ID = -
isEmpty(DD_COMMIT_TIME): DD_COMMIT_TIME = -
DEFINES *= \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
CONFIG(static, static|shared): DEFINES *= BUILD_DD_STATIC
CONFIG(static_ffmpeg) {
    DEFINES *= \
        BUILD_QTAV_STATIC \
        BUILD_QTAVWIDGETS_STATIC
    isEmpty(ffmpeg_dir): ffmpeg_dir = $${ROOT}/ffmpeg
    !exists($${ffmpeg_dir}): error("Can\'t find FFmpeg dir.")
    contains(QT_ARCH, x86_64): LIBS *= -L$${ffmpeg_dir}/lib/x64
    else: LIBS *= -L$${ffmpeg_dir}/lib/x86
    LIBS *= \
        -llibswresample \
        -llibavfilter \
        -llibavdevice -lgdi32 -loleaut32 -lshlwapi \
        -llibavcodec -llibavformat -llibswscale -llibavutil -lws2_32 -lstrmiids -lVfw32 -luuid -lSecur32 -lBcrypt -llegacy_stdio_definitions \
        -lgdiplus -lmfx
}
QT *= \
    widgets \
    network \
    opengl
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
!contains(QT_CONFIG, draganddrop): DEFINES *= DD_NO_DRAG_DROP
!contains(QT_CONFIG, mimetype): DEFINES *= DD_NO_MIME_TYPE
!contains(QT_CONFIG, tooltip): DEFINES *= DD_NO_TOOLTIP
!contains(QT_CONFIG, cssparser) {
    DEFINES *= DD_NO_CSS
} else {
    RESOURCES *= skins.qrc
    HEADERS *= skinsmanager.h
    SOURCES *= skinsmanager.cpp
}
!contains(QT_CONFIG, menu) {
    DEFINES *= DD_NO_MENU
} else {
    FORMS *= forms/traymenu.ui
    HEADERS *= forms/traymenu.h
    SOURCES *= forms/traymenu.cpp
}
!contains(QT_CONFIG, wheelevent): DEFINES *= DD_NO_WHEEL_EVENT
!contains(QT_CONFIG, translation) {
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
        versionAtLeast(QT_VERSION, 5.12.0) {
            CONFIG *= lrelease
        } else {
            isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease.exe
            exists("$${lrelease}") {
                system("$${lrelease} -compress -nounfinished -removeidentical $${_PRO_FILE_}")
            } else {
                message("qmake can\'t find \"lrelease.exe\" in \"$$[QT_INSTALL_BINS]\".")
            }
        }
    }
    versionAtLeast(QT_VERSION, 5.12.0): CONFIG *= embed_translations
    else: RESOURCES *= translations.qrc
}
!contains(QT_CONFIG, commandlineparser): DEFINES *= DD_NO_COMMANDLINE_PARSER
LIBS *= \
    -lUser32 \
    -lDwmapi
TEMPLATE = app
include(../ddutils/ddutils.pri)
include(../3rdparty/qtniceframelesswindow/qtniceframelesswindow.pri)
include(../3rdparty/wallpaper/wallpaper.pri)
include(../3rdparty/qtsingleapplication/qtsingleapplication.pri)
include(../3rdparty/qtav/av.pri)
include(../3rdparty/qtav/avwidgets.pri)
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
