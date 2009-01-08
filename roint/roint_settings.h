/* $Id$ */
#ifndef __ROINT_SETTINGS_H
#define __ROINT_SETTINGS_H

// Use XML?
#define ROINT_USE_XML

// zLib tweaks
#define ZLIB_WINAPI
#pragma comment(lib, "zlib.dll.lib")


// No messing around from this point
#ifdef ROINT_USE_XML
#	define TIXML_USE_STL
#	include "tinyxml/tinyxml.h"
#endif

#endif /* __ROINT_SETTINGS_H */