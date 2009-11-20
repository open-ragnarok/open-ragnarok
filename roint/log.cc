/* $Id$
 * log.cc
 *
 *  Created on: 2009-10-06
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "log.h"

namespace ro {

#ifdef USE_LOGSYS
#ifdef DEBUG
#	define DEBUGLOG true
#else
#	define DEBUGLOG false
#endif
// Checkout logsys.h for more info.
struct LogSysInfo roint_logsys[] = {
	{ ROINT__DEBUG, "ROINT", DEBUGLOG, NULL },
	{ ROINT__TRACE, "RO", false, NULL },
	{ ROINT__ERROR, "RoError", true, stderr },
	{ ROINT__WARNING, "RoWarning", true, stderr }
};

#undef DEBUGLOG
#endif

void SetupLogs() {
#ifdef USE_LOGSYS
#ifdef DEBUG
	printf("ROINT Setting up logs...\n");
	fflush(stdout);
#endif
	LogSys* log = LogSys::GetSingleton();
	log->registerInfo(roint_logsys, 3);
#ifdef DEBUG
	printf("ROINT Log initialized\n");
	fflush(stdout);
#endif
#endif
}

} /* namespace ro */
