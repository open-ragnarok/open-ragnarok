#include "stdafx.h"

#include "ro/common.h"

RO::CDir RO::nextDirection(const CDir& dir) {
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

RO::CDir RO::prevDirection(const CDir& dir) {
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
