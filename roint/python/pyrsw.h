/* $Id$ */
#ifndef __PYRSW_H
#define __PYRSW_H

#include "../rsw.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI RSW* new_RSW();
	ROINT_DLLAPI void del_RSW(RSW*);

	ROINT_DLLAPI bool read_RSW(RSW*, const char*);
	ROINT_DLLAPI void dump_RSW(const RSW*, const char*);
	ROINT_DLLAPI bool write_RSW(const RSW*, const char*);
	ROINT_DLLAPI void clear_RSW(RSW*);

	ROINT_DLLAPI char* get_RSW_ini(RSW*);
	ROINT_DLLAPI char* get_RSW_gnd(RSW*);
	ROINT_DLLAPI char* get_RSW_gat(RSW*);
	ROINT_DLLAPI char* get_RSW_scr(RSW*);
} // Extern "C"
} // namespace RO

#endif /* __PYRSW_H */