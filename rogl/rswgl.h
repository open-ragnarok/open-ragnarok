/* $Id$ */
#ifndef __RSWGL_H
#define __RSWGL_H

#include "globject.h"

#include "rsw.h"
#include "gnd.h"

namespace ROGL {
	MYLIB_DLLAPI bool draw(const RO::GND*, const unsigned int* textures = NULL);
	MYLIB_DLLAPI bool draw(const RO::RSW*, const RO::GND*, const unsigned int* textures = NULL);
}

#endif /* __RSMGL_H */
