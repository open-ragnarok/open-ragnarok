/* $Id$ */
#ifndef __PYACT_H
#define __PYACT_H

#include "../types/act.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI ACT* ACT_new();
	ROINT_DLLAPI void ACT_del(ACT*);

	ROINT_DLLAPI bool ACT_read(ACT*, const char*);

	ROINT_DLLAPI void ACT_dump(const ACT*, const char*);
	ROINT_DLLAPI unsigned int ACT_count(const ACT*);
}
}

#endif /* __PYACT_H */
