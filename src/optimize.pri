# This allows our program to use up to 3 GB memory on 32-bit systems and
# 4 GB memory on 64-bit systems, rather than being limited to just 2 GB.
CONFIG(enable_largeaddressaware) {
    *g++*:        QMAKE_LFLAGS *= -Wl,--large-address-aware
    win32:!*g++*: QMAKE_LFLAGS *= /LARGEADDRESSAWARE
}

# Optimize for size instead of speed
CONFIG(enable_small) {
    # Microsoft Visual C++ Compiler
    win32-msvc* {
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_SIZE): QMAKE_CFLAGS_OPTIMIZE_SIZE = -O1
        isEmpty(QMAKE_CFLAGS_OPTIMIZE): QMAKE_CFLAGS_OPTIMIZE           = -O2
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE                                           *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CXXFLAGS_RELEASE                                         *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                            *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                          *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
    }
    # Intel C++ Compiler and/or GNU C Collection
    win32-icc*|*g++* {
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_SIZE): QMAKE_CFLAGS_OPTIMIZE_SIZE = -O1
        isEmpty(QMAKE_CFLAGS_OPTIMIZE): QMAKE_CFLAGS_OPTIMIZE           = -O2
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_FULL): QMAKE_CFLAGS_OPTIMIZE_FULL = -O3
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_FULL, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_FULL, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_FULL, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_FULL, $$QMAKE_CFLAGS_OPTIMIZE_SIZE)
        QMAKE_CFLAGS_RELEASE                                           *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CXXFLAGS_RELEASE                                         *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                            *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                          *= $$QMAKE_CFLAGS_OPTIMIZE_SIZE
    }
}

# Enable highest optimization level
CONFIG(enable_optimize) {
    # Microsoft Visual C++ Compiler
    win32-msvc* {
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_SIZE): QMAKE_CFLAGS_OPTIMIZE_SIZE = -O1
        isEmpty(QMAKE_CFLAGS_OPTIMIZE): QMAKE_CFLAGS_OPTIMIZE           = -O2
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE)
        QMAKE_CFLAGS_RELEASE                                           *= $$QMAKE_CFLAGS_OPTIMIZE
        QMAKE_CXXFLAGS_RELEASE                                         *= $$QMAKE_CFLAGS_OPTIMIZE
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                            *= $$QMAKE_CFLAGS_OPTIMIZE
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                          *= $$QMAKE_CFLAGS_OPTIMIZE
    }
    # Intel C++ Compiler and/or GNU C Collection
    win32-icc*|*g++* {
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_SIZE): QMAKE_CFLAGS_OPTIMIZE_SIZE = -O1
        isEmpty(QMAKE_CFLAGS_OPTIMIZE): QMAKE_CFLAGS_OPTIMIZE           = -O2
        isEmpty(QMAKE_CFLAGS_OPTIMIZE_FULL): QMAKE_CFLAGS_OPTIMIZE_FULL = -O3
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CFLAGS_RELEASE                                            = $$replace(QMAKE_CFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CXXFLAGS_RELEASE                                          = $$replace(QMAKE_CXXFLAGS_RELEASE, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                             = $$replace(QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                           = $$replace(QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO, $$QMAKE_CFLAGS_OPTIMIZE_SIZE, $$QMAKE_CFLAGS_OPTIMIZE_FULL)
        QMAKE_CFLAGS_RELEASE                                           *= $$QMAKE_CFLAGS_OPTIMIZE_FULL
        QMAKE_CXXFLAGS_RELEASE                                         *= $$QMAKE_CFLAGS_OPTIMIZE_FULL
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO                            *= $$QMAKE_CFLAGS_OPTIMIZE_FULL
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO                          *= $$QMAKE_CFLAGS_OPTIMIZE_FULL
    }
}

# Enable Whole Program Optimization and Link Time Code Generation
CONFIG(enable_ltcg) {
    # Microsoft Visual C++ Compiler
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
    # Intel C++ Compiler
    win32-icc* {
        isEmpty(QMAKE_CFLAGS_LTCG): QMAKE_CFLAGS_LTCG     = -Qipo
        isEmpty(QMAKE_CXXFLAGS_LTCG): QMAKE_CXXFLAGS_LTCG = $$QMAKE_CFLAGS_LTCG
        isEmpty(QMAKE_LFLAGS_LTCG): QMAKE_LFLAGS_LTCG     = $$QMAKE_CFLAGS_LTCG
        QMAKE_CFLAGS_RELEASE                             *= $$QMAKE_CFLAGS_LTCG
        QMAKE_CXXFLAGS_RELEASE                           *= $$QMAKE_CXXFLAGS_LTCG
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO              *= $$QMAKE_CFLAGS_LTCG
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO            *= $$QMAKE_CXXFLAGS_LTCG
        QMAKE_LFLAGS_RELEASE                             *= $$QMAKE_LFLAGS_LTCG
    }
    # Clang
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
    # GNU C Collection
    *g++* {
        isEmpty(QMAKE_CFLAGS_LTCG): QMAKE_CFLAGS_LTCG     = -flto -fno-fat-lto-objects
        isEmpty(QMAKE_CXXFLAGS_LTCG): QMAKE_CXXFLAGS_LTCG = $$QMAKE_CFLAGS_LTCG
        isEmpty(QMAKE_LFLAGS_LTCG): QMAKE_LFLAGS_LTCG     = $$QMAKE_CFLAGS_LTCG -fuse-linker-plugin
        QMAKE_CFLAGS_RELEASE                             *= $$QMAKE_CFLAGS_LTCG
        QMAKE_CXXFLAGS_RELEASE                           *= $$QMAKE_CXXFLAGS_LTCG
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO              *= $$QMAKE_CFLAGS_LTCG
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO            *= $$QMAKE_CXXFLAGS_LTCG
        QMAKE_LFLAGS_RELEASE                             *= $$QMAKE_LFLAGS_LTCG
    }
}
