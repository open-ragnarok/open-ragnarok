/* $Id$ */
#ifndef __PYRSW_H
#define __PYRSW_H

#include "../rsw.h"

namespace RO {
extern "C" {
	MYLIB_DLLAPI RSW* new_RSW();
	MYLIB_DLLAPI void del_RSW(RSW*);

	MYLIB_DLLAPI bool read_RSW(RSW*, const char*);
	MYLIB_DLLAPI void dump_RSW(const RSW*, const char*);
	MYLIB_DLLAPI bool write_RSW(const RSW*, const char*);
	MYLIB_DLLAPI void clear_RSW(RSW*);

	MYLIB_DLLAPI char* get_RSW_ini(RSW*);
	MYLIB_DLLAPI char* get_RSW_gnd(RSW*);
	MYLIB_DLLAPI char* get_RSW_gat(RSW*);
	MYLIB_DLLAPI char* get_RSW_scr(RSW*);
} // Extern "C"
} // namespace RO

#endif /* __PYRSW_H */