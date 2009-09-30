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
#ifndef __PYRSW_H
#define __PYRSW_H

#include "../types/rsw.h"

namespace RO {
extern "C" {
	ROINT_DLLAPI RSW* new_RSW();
	ROINT_DLLAPI void del_RSW(RSW*);

	ROINT_DLLAPI bool read_RSW(RSW*, const char*);
	ROINT_DLLAPI void dump_RSW(const RSW*, const char*);
	ROINT_DLLAPI bool write_RSW(const RSW*, const char*);
	ROINT_DLLAPI void clear_RSW(RSW*);

	ROINT_DLLAPI char* get_RSW_ini(RSW*);
	ROINT_DLLAPI char* get_RSW_gnd(RSW*);
	ROINT_DLLAPI char* get_RSW_gat(RSW*);
	ROINT_DLLAPI char* get_RSW_scr(RSW*);
} // Extern "C"
} // namespace RO

#endif /* __PYRSW_H */
