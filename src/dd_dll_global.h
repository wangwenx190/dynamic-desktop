#pragma once

#ifdef BUILD_SHARED_LIBRARY_DD
    #define DD_SHARED_EXPORT __declspec(dllexport)
#else
    #define DD_SHARED_EXPORT __declspec(dllimport)
#endif
