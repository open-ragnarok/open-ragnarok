/* $Id$ */
#ifndef __ROINT_IMPORT_H
#define __ROINT_IMPORT_H

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#include "roint_settings.h"

#ifndef __WINDOWS__
#	if defined(WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#		define __WINDOWS__
#	endif
#endif


#ifndef __WINDOWS__
	#define ROINT_DLLAPI
#else
	#ifdef ROINT_STATIC
		#define ROINT_DLLAPI
	#else
		#ifdef _ROINT_DLLAPI_
		    #define ROINT_DLLAPI  __declspec( dllexport )
		#else
		    #define ROINT_DLLAPI  __declspec( dllimport )
		#endif
	#endif

// The following will ensure that when building an application (or another
// DLL) using this DLL, the appropriate .LIB file will automatically be used
// when linking.

	#ifndef _ROINT_NOAUTOLIB_
		#ifdef ROINT_STATIC
			#pragma comment(lib, "roint.lib")
		#else
			#pragma comment(lib, "roint.dll.lib")
		#endif
	#endif
#endif

// Use XML?
#ifdef ROINT_USE_XML
#	define TIXML_USE_STL
#	include "tinyxml/tinyxml.h"
#endif

// zLib tweak
#ifdef __WINDOWS__
#	define ZLIB_WINAPI
#endif

#endif /* __ROINT_IMPORT_H */
