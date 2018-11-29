#pragma once

#ifdef BUILD_SHARED_LIBRARY_DD_MAIN
    #define DD_SHARED_EXPORT __declspec(dllexport)
#else
    #define DD_SHARED_EXPORT __declspec(dllimport)
#endif
