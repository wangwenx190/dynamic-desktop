#pragma once

#ifdef BUILD_INNER_SHARED_LIBRARY_DD
    #define DD_INNER_SHARED_EXPORT __declspec(dllexport)
#else
    #define DD_INNER_SHARED_EXPORT __declspec(dllimport)
#endif
