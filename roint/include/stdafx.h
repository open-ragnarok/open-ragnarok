/* $Id$ */
//#pragma once
#ifndef __STDAFX_H
#define __STDAFX_H

#define TIXML_USE_STL

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	ifndef ROINT_DLL
#		pragma comment(lib, "zlib.lib")
#	else
#		pragma comment(lib, "zlib.dll.lib")
#	endif
#endif

#include "ro/roint_settings.h"

#define ROINT_INTERNAL

#include "ro/roint_import.h"

#include "ro/ro.h"

#endif /* __STDAFX_H */
