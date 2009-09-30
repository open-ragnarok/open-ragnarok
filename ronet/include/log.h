#ifndef __LOG_H
#define __LOG_H

#define RONET__DEBUG 13001
#define RONET__ERROR 13002
#define RONET__TRACE 13003

#ifndef USE_LOGSYS
#define _log(id, data, ...)
#define _hexlog(id, data, size)
#else
#define _log(id, data, ...) LogSys::GetSingleton()->log(id, data, __VA_ARGS__)
#define _hexlog(id, data, size) LogSys::GetSingleton()->hexlog(id, data, size)
#endif

#endif /* __LOG_H */