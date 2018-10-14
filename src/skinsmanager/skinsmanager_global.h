#pragma once

#include <QtCore/qglobal.h>

#if defined(SKINSMANAGER_LIBRARY)
#  define SKINSMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SKINSMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif
