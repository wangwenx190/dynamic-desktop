INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
LIBS += -lUser32
HEADERS += \
    $$PWD/qtservice.h \
    $$PWD/qtservice_p.h
SOURCES += \
    $$PWD/qtservice.cpp \
    $$PWD/qtservice_win.cpp
