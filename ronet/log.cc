#include "stdafx.h"

namespace ronet {

void ConfigureLog() {
#ifdef USE_LOGSYS
	FILE* fp = fopen("network.txt", "w");
	LogSys* log = LogSys::GetSingleton();

	log->addHandler(RONET__DEBUG, "NetDebug");
	log->addHandler(RONET__ERROR, "NetError");
	log->addHandler(RONET__TRACE, "Net");

	log->setOutput(RONET__TRACE, fp);
	log->setOutput(RONET__ERROR, stderr);
#endif
}

}