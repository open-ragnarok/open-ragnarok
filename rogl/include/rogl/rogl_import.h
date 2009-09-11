/* $Id$ */
#ifndef __ROGL_IMPORT_H
#define __ROGL_IMPORT_H

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#ifndef _MSC_VER
#	define ROGL_DLLAPI
#else
#	include <windows.h>
#	ifndef ROGL_DLL
#		define ROGL_DLLAPI
#	else
#		ifdef ROGL_INTERNAL
#			define ROGL_DLLAPI  __declspec( dllexport )
#		else
#			define ROGL_DLLAPI  __declspec( dllimport )
#		endif
#	endif
#endif

#ifdef ROGL_DLL
#	define ROINT_DLL
#endif

#include "ro/ro.h"

#endif /* __ROGL_IMPORT_H */
