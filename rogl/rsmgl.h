/* $Id$ */
#ifndef __RSMGL_H
#define __RSMGL_H

#include "globject.h"

#include "rsm.h"

namespace ROGL {
	MYLIB_DLLAPI bool draw(const RO::RSM*, const unsigned int* textures = NULL, int time = 0);
	MYLIB_DLLAPI bool drawBoundingBox(const RO::RSM*);
	MYLIB_DLLAPI bool drawMesh(const unsigned int& meshidx, const RO::RSM*, const unsigned int* textures = NULL);
}
#endif /* __RSMGL_H */