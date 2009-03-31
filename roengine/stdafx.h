/* $Id$ */
#ifndef __STDAFX_H
#define __STDAFX_H

#define __ROENGINE_NO_AUTOIMPORT_LIB__
#define _CRT_SECURE_NO_WARNINGS

#ifdef WIN32
#	include <windows.h>
#	pragma warning(disable: 4251)
#endif

#ifndef ROENGINE_STATIC
#	include "ro/ro.h"
#	include "rogl/rogl.h"
#else
#	include "ro/ro_static.h"
#	include "rogl/rogl_static.h"
#endif

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "sdl.lib")

#endif /* __STDAFX_H */
