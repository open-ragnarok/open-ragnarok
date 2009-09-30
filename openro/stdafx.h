/* $Id$ */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4251)
#endif

#define USE_LOGSYS

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
#else
#	include <SDL/SDL.h>
#endif

#ifdef USE_LOGSYS
#	include "logsys/logsys.h"
#	pragma comment(lib, "logsys.lib")
#endif

#include "log.h"
