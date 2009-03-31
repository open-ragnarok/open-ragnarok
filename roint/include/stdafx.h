/* $Id$ */
//#pragma once
#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef WIN32
#	define _CRT_SECURE_NO_WARNINGS
#	ifdef ROINT_STATIC
#		pragma comment(lib, "zlib.lib")
#	else
#		pragma comment(lib, "zlib.dll.lib")
#	endif
#endif

#include "ro/roint_settings.h"

#define _ROINT_DLLAPI_
#define _ROINT_NOAUTOLIB_

#include "ro/roint_import.h"

#include "ro/ro.h"

#endif /* __STDAFX_H */
