include(../common.pri)
DESTDIR = $${BIN_DIR}
QMAKE_TARGET_PRODUCT = Dynamic Desktop
QMAKE_TARGET_DESCRIPTION = Dynamic Desktop Tray
RC_ICONS = ../resources/icons/color_palette.ico
TARGET = ddtry
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT *= widgets
include(../settingsmanager/settingsmanager.pri)
include(../utils/utils.pri)
HEADERS += forms\traymenu.h
SOURCES += \
    tray.cpp \
    forms\traymenu.cpp
FORMS += forms\traymenu.ui
RESOURCES += images.qrc
TRANSLATIONS += \
    ../resources/translations/try_en.ts \
    ../resources/translations/try_zh_CN.ts
include(../translations.pri)
include(../deploy.pri)
