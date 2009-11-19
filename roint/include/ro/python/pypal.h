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
#ifndef __RO_PYTHON_PYPAL_H
#define __RO_PYTHON_PYPAL_H

#include "../types/pal.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI PAL* PAL_new();
	ROINT_DLLAPI void PAL_del(PAL* pal);
	ROINT_DLLAPI void PAL_copy(PAL* pal, const PAL* other);

	ROINT_DLLAPI unsigned char PAL_read(PAL* pal, const char* fn);
	ROINT_DLLAPI unsigned char PAL_isValid(const PAL* pal);
	ROINT_DLLAPI const PAL::Color* PAL_getColor(const PAL* pal, unsigned char idx);
}
}

#endif /* __RO_PYTHON_PYPAL_H */
