/* $Id: stdafx.h 61 2009-07-22 14:20:16Z sergio $ */
#ifndef __STDAFX_H
#define __STDAFX_H

#define __ROENGINE_NO_AUTOIMPORT_LIB__

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	include <windows.h>
#	include <SDL.h>
#	pragma warning(disable: 4251)
#else
#	include <SDL/SDL.h>
#	include "sdle/sdl_engine.h"
#endif

#ifndef ROENGINE_STATIC
#	include "ro/ro.h"
#	include "rogl/rogl.h"
#else
#	include "ro/ro_static.h"
#	include "rogl/rogl_static.h"
#endif

#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	pragma comment(lib, "sdl.lib")
#endif

#endif /* __STDAFX_H */
