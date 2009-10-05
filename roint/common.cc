/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "stdafx.h"

#include "ro/common.h"

#include <math.h>

namespace RO {

CDir dir2Cdir(float x, float y) {
	float size = sqrt(x*x + y*y);

	x /= size;
	y /= size;

	float dot = y * -1;
	float angle = acos(dot) * 180 / 3.1415f;

	if (x < 0)
		angle = 360 - angle;

	if (angle <= 22.5)
		return(RO::DIR_N);
	else if (angle <= 67.5)
		return(RO::DIR_NE);
	else if (angle <= 112.5)
		return(RO::DIR_E);
	else if (angle <= 157.5)
		return(RO::DIR_SE);
	else if (angle <= 202.5)
		return(RO::DIR_S);
	else if (angle <= 247.5)
		return(RO::DIR_SW);
	else if (angle <= 292.5)
		return(RO::DIR_W);
	else if (angle <= 337.5)
		return(RO::DIR_NW);

	return(RO::DIR_N);
}

CDir nextDirection(CDir dir) {
	switch (dir) {
		case DIR_S:
			return(DIR_SW);
		case DIR_SW:
			return(DIR_W);
		case DIR_W:
			return(DIR_NW);
		case DIR_NW:
			return(DIR_N);
		case DIR_N:
			return(DIR_NE);
		case DIR_NE:
			return(DIR_E);
		case DIR_E:
			return(DIR_SE);
		case DIR_SE:
			return(DIR_S);
	}
	return(DIR_S);
}

CDir prevDirection(CDir dir) {
	switch (dir) {
		case DIR_S:
			return(DIR_SE);
		case DIR_SW:
			return(DIR_S);
		case DIR_W:
			return(DIR_SW);
		case DIR_NW:
			return(DIR_W);
		case DIR_N:
			return(DIR_NW);
		case DIR_NE:
			return(DIR_N);
		case DIR_E:
			return(DIR_NE);
		case DIR_SE:
			return(DIR_E);
	}
	return(DIR_S);
}

}
