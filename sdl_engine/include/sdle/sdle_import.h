#ifndef __SDLENGINE_IMPORT_H
#define __SDLENGINE_IMPORT_H

#ifndef _MSC_VER
#	ifdef SDLENGINE_INTERNAL
#		define SDLENGINE_DLLAPI extern
#	else
#		define SDLENGINE_DLLAPI
#	endif
#else
#	ifndef SDLENGINE_DLL
#		define SDLENGINE_DLLAPI
#	else
#		ifdef SDLENGINE_INTERNAL
#			define SDLENGINE_DLLAPI  __declspec( dllexport )
#		else
#			define SDLENGINE_DLLAPI __declspec( dllimport )
#		endif /* SDLENGINE_INTERNAL */
#	endif /* SDLENGINE_DLL */
#endif

#endif /* __SDLENGINE_IMPORT_H */