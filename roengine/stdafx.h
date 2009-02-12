/* $Id$ */
#ifndef __STDAFX_H
#define __STDAFX_H

#define __ROENGINE_NO_AUTOIMPORT_LIB__
#define _CRT_SECURE_NO_WARNINGS

#ifdef WIN32
#	include <windows.h>
#endif

#ifndef ROENGINE_STATIC
#	include "ro.h"
#	include "rogl.h"
#else
#	include "ro_static.h"
#	include "rogl_static.h"
#endif

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "sdl.lib")

#endif /* __STDAFX_H */
