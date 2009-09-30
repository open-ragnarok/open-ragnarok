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
#ifndef __PYSPR_H
#define __PYSPR_H

#include "../types/spr.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI SPR* new_SPR();
	ROINT_DLLAPI void del_SPR(SPR*);
	ROINT_DLLAPI bool read_SPR(SPR*, const char*);
	ROINT_DLLAPI unsigned int getImgCount_SPR(const SPR*);
	ROINT_DLLAPI bool saveBMP_SPR(const SPR*, int idx, const char* fn);
	ROINT_DLLAPI const SPR::Pal* getPal_SPR(const SPR*, int idx);
	ROINT_DLLAPI const SPR::Image* getFrame_SPR(const SPR*, int idx);
}
}

#endif /* __PYSPR_H */

