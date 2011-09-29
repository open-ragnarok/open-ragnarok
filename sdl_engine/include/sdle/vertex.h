/* $Id$
 * vertex.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */

#ifndef __SDLENGINE_VERTEX_H
#define __SDLENGINE_VERTEX_H

#include "sdle_import.h"

namespace sdle {

class SDLENGINE_DLLAPI Vertex {
public:
	Vertex();
	Vertex(float, float, float);

	float x, y, z;
};

extern const Vertex Vertex_X, Vertex_Y, Vertex_Z, Vertex_ZERO;

}

#endif /* __SDLENGINE_VERTEX_H */
