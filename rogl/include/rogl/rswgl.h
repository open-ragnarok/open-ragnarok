/* $Id$ */
#ifndef __RSWGL_H
#define __RSWGL_H

#include "rogl/globject.h"

#include "ro/types/rsw.h"
#include "ro/types/gnd.h"

namespace rogl {
	ROGL_DLLAPI bool draw(const RO::GND*, const unsigned int* textures = NULL);
	ROGL_DLLAPI bool draw(const RO::RSW*, const RO::GND*, const unsigned int* textures = NULL);
}

#endif /* __RSMGL_H */
