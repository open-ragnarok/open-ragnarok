/*
 * log.h
 *
 *  Created on: 06/10/2009
 *      Author: Sergio
 */

#ifndef LOG_H_
#define LOG_H_

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

#define ROINT__DEBUG 10001
#define ROINT__TRACE 10002
#define ROINT__ERROR 10003
#define ROINT__WARNING 10004

namespace ro {
	ROINT_DLLAPI void SetupLogs();
}

#endif /* LOG_H_ */
