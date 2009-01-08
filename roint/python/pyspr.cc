/* $Id$ */
#include "stdafx.h"

#include "pyspr.h"

namespace RO {
	SPR* new_SPR() {
		return(new SPR());
	}

	void del_SPR(SPR* spr) {
		delete(spr);
	}

	bool read_SPR(SPR* spr, const char* fn) {
		return(spr->read(fn));
	}

	unsigned int getImgCount_SPR(const SPR* spr) {
		return(spr->getImgCount());
	}

	bool saveBMP_SPR(const SPR* spr, int idx, const char* fn) {
		return(spr->saveBMP(idx, fn));
	}

	const SPR::Pal* getPal_SPR(const SPR* spr, int idx) {
		return(spr->getPal(idx));
	}

	const SPR::Image* getFrame_SPR(const SPR* spr, int idx) {
		return(spr->getFrame(idx));
	}
}

