#pragma once

#if defined(WALLPAPER_LIBRARY)
#  define WALLPAPERSHARED_EXPORT __declspec(dllexport)
#else
#  define WALLPAPERSHARED_EXPORT __declspec(dllimport)
#endif
