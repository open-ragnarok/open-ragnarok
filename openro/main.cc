/* $Id$ */
#include "stdafx.h"
#include "openro.h"

#include <stdio.h>

#include <SDL_main.h>

#include "ronet/connection.h"

#define OUTFN "output.txt"
//#define REDIRECT_OUTPUT

int openro_main(int argc, char** argv) {
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


/////////////////////////////////////////////////////////////////////
// replacement code for SDL_main

#ifdef main
#	undef main
#endif

/* SDL_Quit() shouldn't be used with atexit() directly because
   calling conventions may differ... */
static void cleanup(void)
{
	SDL_Quit();
}

int main(int argc, char** argv) {
#ifdef _MSC_VER // SDL_main
	/* Start up DDHELP.EXE before opening any files, so DDHELP doesn't
	   keep them open.  This is a hack.. hopefully it will be fixed 
	   someday.  DDHELP.EXE starts up the first time DDRAW.DLL is loaded.
	 */
	HINSTANCE handle = LoadLibrary(TEXT("DDRAW.DLL"));
	if ( handle != NULL )
		FreeLibrary(handle);
#endif /* _MSC_VER */

	/* Load SDL dynamic link library */
	if ( SDL_Init(SDL_INIT_NOPARACHUTE) < 0 ) {
		fprintf(stderr, "SDL init error: %s", SDL_GetError());
		return(1);
	}
	atexit(cleanup);

#ifdef _MSC_VER // SDL_main
	/* Sam:
	   We still need to pass in the application handle so that
	   DirectInput will initialize properly when SDL_RegisterApp()
	   is called later in the video initialization.
	 */
	SDL_SetModuleHandle(GetModuleHandle(NULL));
#endif
	
	/* Run the application main() code */
	int status = openro_main(argc, argv);

	/* Exit cleanly, calling atexit() functions */
	exit(status);

	/* Hush little compiler, don't you cry... */
	return(0);
}
