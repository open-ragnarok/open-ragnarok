/* $Id$
 * log.cc
 *
 *  Created on: 2009-10-06
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"

#ifdef USE_LOGSYS
#ifdef DEBUG
#	define DEBUGLOG true
#else
#	define DEBUGLOG false
#endif
// Checkout logsys.h for more info.
struct LogSysInfo ronet_logsys[] = {
	{ ROINT__DEBUG, "ROINT", DEBUGLOG, NULL },
	{ ROINT__TRACE, "RO", false, NULL },
	{ ROINT__ERROR, "RoError", true, stderr },
	{ ROINT__WARNING, "RoWarning", true, stderr }
};

#undef DEBUGLOG
#endif

void ro::SetupLogs() {
#ifdef USE_LOGSYS
	LogSys* log = LogSys::GetSingleton();
	log->registerInfo(ronet_logsys, 3);
#endif
}
