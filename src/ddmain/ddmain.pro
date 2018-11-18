TARGET = ddmain
TEMPLATE = app
CONFIG *= lrelease
include(../common.pri)
isEmpty(DD_COMMIT_ID): DD_COMMIT_ID = -
isEmpty(DD_COMMIT_TIME): DD_COMMIT_TIME = -
DEFINES *= \
    DD_COMMIT_ID=\\\"$${DD_COMMIT_ID}\\\" \
    DD_COMMIT_TIME=\\\"$${DD_COMMIT_TIME}\\\"
CONFIG(static, static|shared): DEFINES *= BUILD_DD_STATIC
RC_ICONS = ../resources/icons/color_palette.ico
QMAKE_TARGET_DESCRIPTION = "Dynamic Desktop"
CONFIG(static_ffmpeg) {
    isEmpty(ffmpeg_dir): ffmpeg_dir = $${ROOT}/ffmpeg
    !exists($${ffmpeg_dir}): error("Can\'t find FFmpeg dir.")
    isEmpty(ffmpeg_lib_dir): ffmpeg_lib_dir = $${ffmpeg_dir}/lib
    !exists($${ffmpeg_lib_dir}/*avcodec.lib) {
        contains(QT_ARCH, x86_64): ffmpeg_lib_dir = $${ffmpeg_lib_dir}/x64
        else: ffmpeg_lib_dir = $${ffmpeg_lib_dir}/x86
    }
    LIBS *= -L$${ffmpeg_lib_dir}
    LIBS *= \
        -lVfw32 -lgdiplus -llibmfx -lSecur32 \
        -lBcrypt -llegacy_stdio_definitions -llibavcodec \
        -llibavdevice -llibavfilter -llibavformat \
        -llibavutil -llibswresample -llibswscale
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
    QMAKE_LUPDATE_FLAGS *= -no-obsolete -locations none -no-ui-lines
    QMAKE_LRELEASE_FLAGS *= -nounfinished -removeidentical
    CONFIG(update_translations): CONFIG *= lupdate lrelease
    CONFIG(lupdate) {
        isEmpty(lupdate): lupdate = $$[QT_INSTALL_BINS]/lupdate
        exists("$${lupdate}") {
            system("$${lupdate} $${QMAKE_LUPDATE_FLAGS} $${_PRO_FILE_}")
        } else {
            message("qmake can\'t find \"lupdate\" in \"$$[QT_INSTALL_BINS]\".")
        }
    }
    CONFIG(lrelease) {
        versionAtLeast(QT_VERSION, 5.12.0) {
            CONFIG *= \
                lrelease \
                embed_translations
        } else {
            isEmpty(lrelease): lrelease = $$[QT_INSTALL_BINS]/lrelease
            exists("$${lrelease}") {
                system("$${lrelease} -nounfinished -removeidentical $${_PRO_FILE_}")
            } else {
                message("qmake can\'t find \"lrelease\" in \"$$[QT_INSTALL_BINS]\".")
            }
            RESOURCES *= translations.qrc
        }
    }
}
#!contains(QT_CONFIG, commandlineparser): DEFINES *= DD_NO_COMMANDLINE_PARSER
LIBS *= \
    -lUser32 \
    -lDwmapi
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
