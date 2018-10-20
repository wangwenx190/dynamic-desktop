LIBS *= -lUser32
INCLUDEPATH += \
    $$PWD \
    $$PWD/forms
DEPENDPATH += \
    $$PWD \
    $$PWD/forms
HEADERS += \
    $$PWD/forms/preferencesdialog.h \
    $$PWD/forms/aboutdialog.h \
    $$PWD/../common.h
SOURCES += \
    $$PWD/controller.cpp \
    $$PWD/forms/preferencesdialog.cpp \
    $$PWD/forms/aboutdialog.cpp
FORMS += \
    $$PWD/forms/preferencesdialog.ui \
    $$PWD/forms/aboutdialog.ui
