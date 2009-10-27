#ifndef __SDLENGINE_STDAFX_H
#define __SDLENGINE_STDAFX_H

#ifdef _MSC_VER // Using Microsoft Visual Studio
#	define _CRT_SECURE_NO_WARNINGS // Disable secure warnings
#	include <windows.h>
#	pragma comment(lib, "sdl.lib")
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	pragma comment(lib, "freetype.lib")
#	ifdef SDLENGINE_DLL
#		pragma comment(lib, "zlib.dll.lib")
#		pragma comment(lib, "libpng.dll.lib")
#	else
#		pragma comment(lib, "zlib.lib")
#		pragma comment(lib, "libpng.lib")
#	endif
#endif

#define SDLENGINE_INTERNAL

#endif /* __SDLENGINE_STDAFX_H */
