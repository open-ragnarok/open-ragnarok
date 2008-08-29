/* $id$ */
#ifndef __IMPDECL_H
#define __IMPDECL_H

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#ifndef WIN32
	#define MYLIB_DLLAPI
#else
	#ifdef _MYLIB_DLLAPI_
	    #define MYLIB_DLLAPI  __declspec( dllexport )
	#else
	    #define MYLIB_DLLAPI  __declspec( dllimport )
	#endif

// The following will ensure that when building an application (or another
// DLL) using this DLL, the appropriate .LIB file will automatically be used
// when linking.

	#ifndef _MYLIB_NOAUTOLIB_
		#pragma comment(lib, "roint.dll.lib")
	#endif
#endif


#endif /* __IMPDECL_H */
