/* $Id$ */
#include "stdafx.h"
#include "openro.h"

#ifdef _MSC_VER
#	include <SDL_main.h>
#else
#	include <SDL/SDL_main.h>
#endif

int main(int argc, char* argv[]) {
	OpenRO engine;
	engine.Run();
	return(0);
}
