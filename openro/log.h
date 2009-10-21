#ifndef __LOG_H
#define __LOG_H

#define OPENRO__DEBUG 15001
#define OPENRO__ERROR 15002
#define OPENRO__TRACE 15003

#ifdef USE_LOGSYS
#	include "logsys/logsys_macros.h"
#	include "logsys/logsys.h"
#else
#	ifdef _MSC_VER
#		define _log(id, data, ...)
#		define _logif(statement, id, data, ...)
#	else
#		define _log(id, data...)
#		define _logif(statement, id, data...)
#	endif
#	define _hexlog(id, data, size)
#endif


#endif /* __LOG_H */
