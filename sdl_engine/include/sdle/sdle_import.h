#ifndef __SDLENGINE_IMPORT_H
#define __SDLENGINE_IMPORT_H

#if defined(SDLENGINE_DLL) && defined(_MSC_VER)
#	ifdef SDLENGINE_INTERNAL
#		define SDLENGINE_DLLAPI  __declspec( dllexport )
#	else
#		define SDLENGINE_DLLAPI __declspec( dllimport )
#	endif /* SDLENGINE_INTERNAL */
#else
#	define SDLENGINE_DLLAPI
#endif /* defined(SDLENGINE_DLL) && defined(_MSC_VER) */

#endif /* __SDLENGINE_IMPORT_H */
