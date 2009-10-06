/* $Id$
 * logsys_macros.h
 *
 *  Created on: 2009-10-06
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __LOGSYS_MACROS_H
#define __LOGSYS_MACROS_H

// Define USE_LOGSYS in your source code before including this file to make these macros work
// otherwise, they'll just default to "nothing" -- wich is also useful when you want to make
// your code work with and without logsys.

#ifndef USE_LOGSYS
#	ifdef _MSC_VER
#		define _log(id, data, ...)
#		define _logif(statement, id, data, ...)
#	else
#		define _log(id, data...)
#		define _logif(statement, id, data...)
#	endif
#	define _hexlog(id, data, size)
#else
#	ifdef _MSC_VER
#		define _log(id, data, ...) LogSys::GetSingleton()->log(id, data, __VA_ARGS__)
#		define _logif(statement, id, data, ...) if (statement) { LogSys::GetSingleton()->log(id, data, __VA_ARGS__); }
#	else
#		define _log(id, data...) LogSys::GetSingleton()->log(id, data)
#		define _logif(statement, id, data...) if (statement) { LogSys::GetSingleton()->log(id, data); }
#	endif
#	define _hexlog(id, data, size) LogSys::GetSingleton()->hexlog(id, data, size)
#endif

#endif /* __LOGSYS_MACROS_H */
