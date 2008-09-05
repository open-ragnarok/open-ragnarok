/* $Id$ */
#ifndef __RSMGL_H
#define __RSMGL_H

#include "globject.h"

#include "rsm.h"

namespace ROGL {
	MYLIB_DLLAPI bool draw(const RO::RSM*, const unsigned int* textures = NULL, int time = 0);

}
#endif /* __RSMGL_H */