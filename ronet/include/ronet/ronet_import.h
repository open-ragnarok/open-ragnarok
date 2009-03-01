/* $Id$ */
#ifndef __RONET_IMPORT_H
#define __RONET_IMPORT_H

#ifndef __WINDOWS__
#	if defined(WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#		define __WINDOWS__
#	endif
#endif

#ifndef __WINDOWS__
	#define RONET_DLLAPI
#else
	#ifdef RONET_STATIC
		#define RONET_DLLAPI
	#else
		#ifdef _RONET_DLLAPI_
		    #define RONET_DLLAPI  __declspec( dllexport )
		#else
		    #define RONET_DLLAPI  __declspec( dllimport )
		#endif
	#endif

// The following will ensure that when building an application (or another
// DLL) using this DLL, the appropriate .LIB file will automatically be used
// when linking.

	#ifndef _RONET_NOAUTOLIB_
		#ifdef RONET_STATIC
			#pragma comment(lib, "ronet.lib")
		#else
			#pragma comment(lib, "ronet.dll.lib")
		#endif
	#endif
#endif

#pragma comment(lib, "wsock32.lib")

#endif /* __RONET_IMPORT_H */