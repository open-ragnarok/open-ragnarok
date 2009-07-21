/* $Id$ */
#ifndef __PYGRF_H
#define __PYGRF_H

#include "../types/grf.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI GRF* GRF_new();
	ROINT_DLLAPI void GRF_del(GRF*);
	ROINT_DLLAPI bool GRF_open(GRF*, const char*);
	ROINT_DLLAPI void GRF_close(GRF*);
	ROINT_DLLAPI bool GRF_isOpen(GRF*);
	ROINT_DLLAPI unsigned int GRF_getCount(const GRF*);
	ROINT_DLLAPI const char* GRF_getFilename(const GRF*, unsigned int i);
}
}

#endif /* __PYGRF_H */
