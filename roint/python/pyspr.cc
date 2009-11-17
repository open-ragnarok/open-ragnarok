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

#include "ro/python/pyspr.h"

namespace RO {
	SPR* new_SPR() {
		return(new SPR());
	}

	void del_SPR(SPR* spr) {
		delete(spr);
	}

	bool read_SPR(SPR* spr, const char* fn) {
		return(spr->read(fn));
	}

	unsigned int getImageCount_SPR(const SPR* spr, int type) {
		return(spr->getImageCount((SPR::ImageType)type));
	}

	const SPR::Image* getImage_SPR(const SPR* spr, int idx, int type) {
		return(spr->getImage(idx, (SPR::ImageType)type));
	}

	const PAL* getPal_SPR(const SPR* spr) {
		return(spr->getPal());
	}

	bool saveBMP_SPR(const SPR* spr, int idx, const char* fn, int type, const PAL* pal) {
		return(spr->saveBMP(idx, fn, (SPR::ImageType)type, pal));
	}
}

