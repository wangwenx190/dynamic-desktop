INCLUDEPATH	+= $$PWD
DEPENDPATH += $$PWD
QT *= network
QT -= gui
HEADERS += \
    $$PWD/qtsinglecoreapplication.h \
    $$PWD/qtlocalpeer.h \
    $$PWD/qtlockedfile.h
SOURCES += \
    $$PWD/qtsinglecoreapplication.cpp \
    $$PWD/qtlocalpeer.cpp \
    $$PWD/qtlockedfile.cpp \
    $$PWD/qtlockedfile_win.cpp
