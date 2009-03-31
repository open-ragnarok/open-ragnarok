/* $Id$ */
#ifndef __ROGL_IMPORT_H
#define __ROGL_IMPORT_H

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#ifndef WIN32
	#define ROGL_DLLAPI
#else
	#include <windows.h>
	#ifdef ROGL_STATIC
		#define ROGL_DLLAPI
	#else
		#ifdef _ROGL_DLLAPI_
			#define ROGL_DLLAPI  __declspec( dllexport )
		#else
		    #define ROGL_DLLAPI  __declspec( dllimport )
		#endif
	#endif

// The following will ensure that when building an application (or another
// DLL) using this DLL, the appropriate .LIB file will automatically be used
// when linking.

	#ifndef _ROGL_NOAUTOLIB_
		#ifdef ROGL_STATIC
			#pragma comment(lib, "rogl.lib")
		#else
			#pragma comment(lib, "rogl.dll.lib")
		#endif
	#endif
#endif

#ifdef ROGL_STATIC
#	include "ro/ro_static.h"
#else
#	include "ro/ro.h"
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#endif /* __ROGL_IMPORT_H */

