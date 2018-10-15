INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT *= widgets network
HEADERS += \
    $$PWD/qtsingleapplication.h \
    $$PWD/qtlocalpeer.h \
    $$PWD/qtlockedfile.h
SOURCES += \
    $$PWD/qtsingleapplication.cpp \
    $$PWD/qtlocalpeer.cpp \
    $$PWD/qtlockedfile.cpp \
    $$PWD/qtlockedfile_win.cpp
