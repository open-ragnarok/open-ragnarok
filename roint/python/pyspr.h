/* $Id$ */
#ifndef __PYSPR_H
#define __PYSPR_H

#include "../spr.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI SPR* new_SPR();
	ROINT_DLLAPI void del_SPR(SPR*);
	ROINT_DLLAPI bool read_SPR(SPR*, const char*);
	ROINT_DLLAPI unsigned int getImgCount_SPR(const SPR*);
	ROINT_DLLAPI bool saveBMP_SPR(const SPR*, int idx, const char* fn);
	ROINT_DLLAPI const SPR::Pal* getPal_SPR(const SPR*, int idx);
	ROINT_DLLAPI const SPR::Image* getFrame_SPR(const SPR*, int idx);
}
}

#endif /* __PYSPR_H */

