/* $Id$ */
#ifndef __RONET_IMPORT_H
#define __RONET_IMPORT_H

#ifndef _MSC_VER
#	define RONET_DLLAPI
#else
#	ifndef RONET_DLL
#		define RONET_DLLAPI
#	else
#		ifdef RONET_INTERNAL
#			define RONET_DLLAPI  __declspec( dllexport )
#		else
#			define RONET_DLLAPI  __declspec( dllimport )
#		endif
#	endif
#endif


#endif /* __RONET_IMPORT_H */
