/* $Id$ */
#include "stdafx.h"

#ifdef _MSC_VER // Using Microsoft Visual Studio
#	define _CRT_SECURE_NO_WARNINGS // Disable secure warnings
#	include <windows.h>
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	ifdef ROGL_DLL
#		pragma comment(lib, "sdlengine.dll.lib")
#	endif
#endif
