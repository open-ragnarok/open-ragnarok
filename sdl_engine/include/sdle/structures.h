/* $Id$
 * structures.h
 *
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLENGINE_STRUCTURES_H
#define __SDLENGINE_STRUCTURES_H

struct Pos2D {
	float x, y;
};

struct Dimension {
	float w, h;
};

struct Rect {
	struct Pos2D pos;
	struct Dimension dim;
};

#endif /* __SDLENGINE_STRUCTURES_H */
