/* $Id$ */
#ifndef __RSMGL_H
#define __RSMGL_H

#include "rogl/globject.h"

#include "ro/types/rsm.h"

namespace rogl {
	ROGL_DLLAPI bool draw(const ro::RSM*, const unsigned int* textures = NULL, int time = 0);
	ROGL_DLLAPI bool drawBoundingBox(const ro::RSM*);
	ROGL_DLLAPI bool drawMesh(const unsigned int& meshidx, const ro::RSM*, const unsigned int* textures = NULL);
}
#endif /* __RSMGL_H */
