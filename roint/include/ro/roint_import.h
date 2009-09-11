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

#ifndef _MSC_VER
#	define ROINT_DLLAPI
#else
#	ifndef ROINT_DLL
#		define ROINT_DLLAPI
#	else
#		ifdef ROINT_INTERNAL
#			define ROINT_DLLAPI  __declspec( dllexport )
#		else
#			define ROINT_DLLAPI  __declspec( dllimport )
#		endif
#	endif
#endif

// Use XML?
#ifdef ROINT_USE_XML
#	define TIXML_USE_STL
#	include "tinyxml/tinyxml.h"
#endif

#endif /* __ROINT_IMPORT_H */
