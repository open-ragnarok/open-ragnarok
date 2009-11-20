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

#include "ro/python/pyrsw.h"

namespace ro {

RSW* new_RSW() {
	return(new RSW());
}

void del_RSW(RSW* rsw) {
	delete(rsw);
}

bool read_RSW(RSW* rsw, const char* filename) {
	return(rsw->read(filename));
}

void dump_RSW(const RSW* rsw, const char* pfx) {
	rsw->Dump(std::cout, pfx);
}

bool write_RSW(const RSW* rsw, const char* filename) {
	std::ofstream fp(filename, std::ios_base::binary);
	bool ret = rsw->writeStream(fp);
	fp.close();
	return(ret);
}

const char* get_RSW_ini(RSW* rsw) {
	return(rsw->getIniFile());
}

const char* get_RSW_gnd(RSW* rsw) {
	return(rsw->getGndFile());
}

const char* get_RSW_gat(RSW* rsw) {
	return(rsw->getGatFile());
}

const char* get_RSW_scr(RSW* rsw) {
	return(rsw->getScrFile());
}

} /* namespace ro */
