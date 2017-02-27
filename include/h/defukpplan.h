#ifndef DEFUKPPLAN_H
#define DEFUKPPLAN_H

#include <QtGlobal>

#if defined(Q_OS_WIN32)
#  if defined(UKPPLAN_LIBRARY)
#    define UKPPLANSHARED_EXPORT Q_DECL_EXPORT
#  else
#    define UKPPLANSHARED_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define UKPPLANSHARED_EXPORT
#endif


#endif // DEFUKPPLAN_H
