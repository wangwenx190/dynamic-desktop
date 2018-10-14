#pragma once

#include <QtCore/qglobal.h>

#if defined(QSIMPLEUPDATER_LIBRARY)
#  define QSIMPLEUPDATERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QSIMPLEUPDATERSHARED_EXPORT Q_DECL_IMPORT
#endif
