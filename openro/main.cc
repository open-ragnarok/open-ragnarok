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
#if defined(WIN32) || defined(_MSC_VER)
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
