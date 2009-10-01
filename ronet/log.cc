#include "stdafx.h"

namespace ronet {

#ifdef USE_LOGSYS
// Checkout logsys.h for more info.
struct LogSysInfo ronet_logsys[] = {
	{ RONET__DEBUG, "NetDebug", true, NULL },
	{ RONET__ERROR, "NetError", true, stderr },
	{ RONET__TRACE, "Net", false, NULL}
};
#endif

void ConfigureLog() {
#ifdef USE_LOGSYS
	FILE* fp = fopen("network.txt", "w");
	ronet_logsys[2].output = fp;

	LogSys* log = LogSys::GetSingleton();
	log->registerInfo(ronet_logsys, 3);
#endif
}

}