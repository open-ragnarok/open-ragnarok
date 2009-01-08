/* $Id$ */
#ifndef __PYGRF_H
#define __PYGRF_H

#include "../grf.h"

namespace RO {
extern "C" {
	MYLIB_DLLAPI GRF* GRF_new();
	MYLIB_DLLAPI void GRF_del(GRF*);
	MYLIB_DLLAPI bool GRF_open(GRF*, const char*);
	MYLIB_DLLAPI void GRF_close(GRF*);
	MYLIB_DLLAPI bool GRF_isOpen(GRF*);
	MYLIB_DLLAPI unsigned int GRF_getCount(const GRF*);
	MYLIB_DLLAPI const char* GRF_getFilename(const GRF*, unsigned int i);
}
}

#endif /* __PYGRF_H */