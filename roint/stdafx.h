/* $Id$ */
//#pragma once
#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "roint_settings.h"

#ifdef ZLIB_STATIC
#	pragma comment(lib, "zlib.lib")
#else
#	pragma comment(lib, "zlib.dll.lib")
#endif

#define _MYLIB_DLLAPI_
#define _MYLIB_NOAUTOLIB_

#include "impdecl.h"

#include "ro.h"

#endif /* __STDAFX_H */
