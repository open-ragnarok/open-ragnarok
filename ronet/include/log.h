#ifndef __LOG_H
#define __LOG_H

#define RONET__DEBUG 13001
#define RONET__ERROR 13002
#define RONET__TRACE 13003

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
