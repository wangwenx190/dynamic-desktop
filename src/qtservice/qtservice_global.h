#pragma once

#include <QtCore/qglobal.h>

#if defined(QTSERVICE_LIBRARY)
#  define QTSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif
