/* $Id$ */
#include "stdafx.h"
#include "openro.h"

#include <stdio.h>

#ifdef _MSC_VER
#	include <SDL_main.h>
#else
#	include <SDL/SDL_main.h>
#endif

#include "ronet/connection.h"

#define OUTFN "output.txt"
//#define REDIRECT_OUTPUT

int main(int argc, char* argv[]) {
#ifdef USE_LOGSYS
	// Checkout logsys.h for more info.
	struct LogSysInfo openro_logsys[] = {
		{ OPENRO__DEBUG, "Debug", true, NULL },
		{ OPENRO__TRACE, "OpenRO", true, NULL },
		{ OPENRO__ERROR, "Error", true, stderr}
	};

	LogSys::GetSingleton()->registerInfo(openro_logsys, 3);

	ronet::ConfigureLog();
#endif

#if defined(WIN32) || defined(_MSC_VER)
	_log(OPENRO__DEBUG, "Initializing Winsock...");
	ronet::InitWinsock winsock;
#endif

#ifdef REDIRECT_OUTPUT
	printf("Redirecting output to %s...\n", OUTFN);
	freopen(OUTFN, "w", stdout);
#endif

	OpenRO engine;
	engine.Run();

#ifdef REDIRECT_OUTPUT
	fclose(stdout);
#endif

	return(0);
}
