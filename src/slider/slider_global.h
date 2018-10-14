#pragma once

#include <QtCore/qglobal.h>

#if defined(SLIDER_LIBRARY)
#  define SLIDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SLIDERSHARED_EXPORT Q_DECL_IMPORT
#endif
