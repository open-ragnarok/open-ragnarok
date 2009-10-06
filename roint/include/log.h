/*
 * log.h
 *
 *  Created on: 06/10/2009
 *      Author: n0c2
 */

#ifndef LOG_H_
#define LOG_H_

#include "logsys/logsys_macros.h"
#ifdef USE_LOGSYS
#	include "logsys/logsys.h"
#endif

#define ROINT__DEBUG 10001
#define ROINT__TRACE 10002
#define ROINT__ERROR 10003
#define ROINT__WARNING 10004

namespace ro {
	void SetupLogs();
}

#endif /* LOG_H_ */
