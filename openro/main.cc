/* $Id$ */
#include "stdafx.h"
#include "openro.h"

#ifdef _MSC_VER
#	include <SDL_main.h>
#else
#	include <SDL/SDL_main.h>
#endif

#include "ronet/connection.h"

int main(int argc, char* argv[]) {
#if defined(WIN32) || defined(_MSC_VER)
	ronet::InitWinsock winsock;
#endif

	OpenRO engine;
	engine.Run();
	return(0);
}
