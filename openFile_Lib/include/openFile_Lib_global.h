#ifndef OPENFILE_LIB_GLOBAL_H
#define OPENFILE_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPENFILE_LIB_LIBRARY)
#  define OPENFILE_LIB_EXPORT Q_DECL_EXPORT
#else
#  define OPENFILE_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // OPENFILE_LIB_GLOBAL_H
