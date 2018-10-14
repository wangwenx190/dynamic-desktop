#pragma once

#include <QtCore/qglobal.h>

#if defined(UTILS_LIBRARY)
#  define UTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILSSHARED_EXPORT Q_DECL_IMPORT
#endif
