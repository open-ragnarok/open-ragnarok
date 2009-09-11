/* $Id$
 * vertex.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/vertex.h"

namespace sdle {

const Vertex Vertex_X(1,0,0);
const Vertex Vertex_Y(0,1,0);
const Vertex Vertex_Z(0,0,1);
const Vertex Vertex_ZERO(0,0,0);

Vertex::Vertex() {
	x = 0;
	y = 0;
	z = 0;
}

Vertex::Vertex(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

}
