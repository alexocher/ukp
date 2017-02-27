#ifndef DEFUKPCOMMON_H
#define DEFUKPCOMMON_H

#include <QtGlobal>

#if defined(Q_OS_WIN32)
#  if defined(UKPCOMMON_LIBRARY)
#    define UKPCOMMONSHARED_EXPORT Q_DECL_EXPORT
#  else
#    define UKPCOMMONSHARED_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define UKPCOMMONSHARED_EXPORT
#endif

#endif // DEFUKPCOMMON_H
