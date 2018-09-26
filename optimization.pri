# This allows our program to use up to 3 GB memory on 32-bit systems and
# 4 GB memory on 64-bit systems, rather than being limited to just 2 GB.
win32-g++*|linux-g++*|macx-g++*:     QMAKE_LFLAGS *= -Wl,--large-address-aware
win32-msvc*|win32-icc*|win32-clang*: QMAKE_LFLAGS *= /LARGEADDRESSAWARE

# Enable Whole Program Optimization and Link Time Code Generation
win32-msvc* {
    isEmpty(QMAKE_CFLAGS_LTCG): QMAKE_CFLAGS_LTCG     = -GL
    isEmpty(QMAKE_CXXFLAGS_LTCG): QMAKE_CXXFLAGS_LTCG = $$QMAKE_CFLAGS_LTCG
    isEmpty(QMAKE_LFLAGS_LTCG): QMAKE_LFLAGS_LTCG     = /LTCG
    QMAKE_CFLAGS_RELEASE                             *= $$QMAKE_CFLAGS_LTCG
    QMAKE_CXXFLAGS_RELEASE                           *= $$QMAKE_CXXFLAGS_LTCG
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO              *= $$QMAKE_CFLAGS_LTCG
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO            *= $$QMAKE_CXXFLAGS_LTCG
    QMAKE_LFLAGS_RELEASE                             *= $$QMAKE_LFLAGS_LTCG
}

win32-icc* {
    isEmpty(QMAKE_CFLAGS_OPTIMIZE): QMAKE_CFLAGS_OPTIMIZE           = -O2
    isEmpty(QMAKE_CFLAGS_OPTIMIZE_FULL): QMAKE_CFLAGS_OPTIMIZE_FULL = -O3
    QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
    QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
    isEmpty(QMAKE_CFLAGS_LTCG): QMAKE_CFLAGS_LTCG                   = -Qipo
    isEmpty(QMAKE_CXXFLAGS_LTCG): QMAKE_CXXFLAGS_LTCG               = $$QMAKE_CFLAGS_LTCG
    isEmpty(QMAKE_LFLAGS_LTCG): QMAKE_LFLAGS_LTCG                   = $$QMAKE_CFLAGS_LTCG
    QMAKE_CFLAGS_RELEASE                                           *= $$QMAKE_CFLAGS_LTCG
    QMAKE_CXXFLAGS_RELEASE                                         *= $$QMAKE_CXXFLAGS_LTCG
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                            *= $$QMAKE_CFLAGS_LTCG
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                          *= $$QMAKE_CXXFLAGS_LTCG
    QMAKE_LFLAGS_RELEASE                                           *= $$QMAKE_LFLAGS_LTCG
}

win32-clang* {
    #isEmpty(QMAKE_CFLAGS_LTCG): QMAKE_CFLAGS_LTCG     = -GL
    #isEmpty(QMAKE_CXXFLAGS_LTCG): QMAKE_CXXFLAGS_LTCG = $$QMAKE_CFLAGS_LTCG
    isEmpty(QMAKE_LFLAGS_LTCG): QMAKE_LFLAGS_LTCG      = /LTCG
    #QMAKE_CFLAGS_RELEASE                             *= $$QMAKE_CFLAGS_LTCG
    #QMAKE_CXXFLAGS_RELEASE                           *= $$QMAKE_CXXFLAGS_LTCG
    #QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO              *= $$QMAKE_CFLAGS_LTCG
    #QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO            *= $$QMAKE_CXXFLAGS_LTCG
    QMAKE_LFLAGS_RELEASE                              *= $$QMAKE_LFLAGS_LTCG
}
