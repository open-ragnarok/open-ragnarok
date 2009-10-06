#ifndef __LOG_H
#define __LOG_H

#define RONET__DEBUG 13001
#define RONET__ERROR 13002
#define RONET__TRACE 13003

#include "logsys/logsys_macros.h"
#ifdef USE_LOGSYS
#	include "logsys/logsys.h"
#endif

#endif /* __LOG_H */