LIBS *= \
    -lWtsapi32 \
    -lUserenv \
    -lAdvapi32
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
SOURCES += $$PWD/service.cpp
