/* $Id$ */
#ifndef __PYACT_H
#define __PYACT_H

#include "../act.h"

namespace RO {
extern "C" {
	MYLIB_DLLAPI ACT* ACT_new();
	MYLIB_DLLAPI void ACT_del(ACT*);

	MYLIB_DLLAPI bool ACT_read(ACT*, const char*);

	MYLIB_DLLAPI void ACT_dump(const ACT*, const char*);
	MYLIB_DLLAPI unsigned int ACT_count(const ACT*);
}
}

#endif /* __PYACT_H */