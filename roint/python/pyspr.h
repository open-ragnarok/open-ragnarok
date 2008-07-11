/* $id$ */
#ifndef __PYSPR_H
#define __PYSPR_H

#include "../spr.h"

namespace RO {
extern "C" {
	MYLIB_DLLAPI SPR* new_SPR();
	MYLIB_DLLAPI void del_SPR(SPR*);
	MYLIB_DLLAPI bool read_SPR(SPR*, const char*);
	MYLIB_DLLAPI unsigned int getImgCount_SPR(const SPR*);
	MYLIB_DLLAPI bool saveBMP_SPR(const SPR*, int idx, const char* fn);
	MYLIB_DLLAPI const SPR::Pal* getPal_SPR(const SPR*, int idx);
	MYLIB_DLLAPI const SPR::Image* getFrame_SPR(const SPR*, int idx);
}
}

#endif /* __PYSPR_H */

