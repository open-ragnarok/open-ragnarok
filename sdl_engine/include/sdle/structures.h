/* $Id$
 * structures.h
 *
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLE_STRUCTURES_H
#define __SDLE_STRUCTURES_H

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

#endif /* __SDLE_STRUCTURES_H */