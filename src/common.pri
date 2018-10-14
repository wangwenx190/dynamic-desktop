CONFIG *= c++11 c++14
DEFINES *= \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050603
include(root.pri)
include(version.pri)
include(optimization.pri)