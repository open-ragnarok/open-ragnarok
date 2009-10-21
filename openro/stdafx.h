/* $Id$ */
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning(disable: 4251)
#	define USE_LOGSYS
#else
#	ifdef DEBUG
#	endif
#endif

#include "ro/ro.h"
#include "rogl/rogl.h"
#include "ronet/ronet.h"

#include "openro.h"
#include "gui/desktops.h"

#ifdef _MSC_VER
#	include <SDL.h>
#	pragma comment(lib, "SDLmain.lib")
#	pragma comment(lib, "sdlengine.lib")
#	pragma comment(lib, "jpeg.lib")
#	ifdef USE_LOGSYS
#		pragma comment(lib, "logsys.lib")
#	endif
#else
#	include <SDL/SDL.h>
#endif

#include "log.h"
