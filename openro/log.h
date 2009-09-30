#ifndef __LOG_H
#define __LOG_H

#define OPENRO__DEBUG 15001
#define OPENRO__ERROR 15002
#define OPENRO__TRACE 15003

#ifndef USE_LOGSYS
#define _log(id, data, ...)
#define _hexlog(id, data, size)
#else
#define _log(id, data, ...) LogSys::GetSingleton()->log(id, data, __VA_ARGS__)
#define _hexlog(id, data, size) LogSys::GetSingleton()->hexlog(id, data, size)
#endif

#endif /* __LOG_H */