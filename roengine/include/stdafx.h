/* $Id: stdafx.h 61 2009-07-22 14:20:16Z sergio $ */
#ifndef __STDAFX_H
#define __STDAFX_H

#define __ROENGINE_NO_AUTOIMPORT_LIB__
#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
#	include <windows.h>
#	include <SDL.h>
#	pragma warning(disable: 4251)
#endif

#ifndef ROENGINE_STATIC
#	include "ro/ro.h"
#	include "rogl/rogl.h"
#else
#	include "ro/ro_static.h"
#	include "rogl/rogl_static.h"
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#ifdef _MSC_VER
#	pragma comment(lib, "sdl.lib")
#endif

#endif /* __STDAFX_H */
