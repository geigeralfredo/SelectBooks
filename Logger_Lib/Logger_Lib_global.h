#ifndef LOGGER_LIB_GLOBAL_H
#define LOGGER_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGGER_LIB_LIBRARY)
#  define LOGGER_LIB_EXPORT Q_DECL_EXPORT
#else
#  define LOGGER_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGGER_LIB_GLOBAL_H
