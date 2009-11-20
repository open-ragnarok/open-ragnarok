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

#include "ro/python/pygrf.h"

#include <fstream>

namespace ro {

GRF* GRF_new() {
	return(new GRF());
}

void GRF_del(GRF* grf) {
	delete(grf);
}

bool GRF_open(GRF* grf, const char* fn) {
	return(grf->open(fn));
}

void GRF_close(GRF* grf) {
	grf->close();
}

bool GRF_isOpen(GRF* grf) {
	return(grf->isOpen());
}

char __grf_getfilename[256];
const char* GRF_getFilename(const GRF* grf, unsigned int i) {
	// Python wouldn't handle deleting the string when it was no longer used, so, I'm using a static variable that
	// will forcibly be cleared out once the dll is unloaded.
	// Be sure to copy the received pointer into some other piece of memory or it will get toasted
	// with the next call of this function.
	std::string fn;
	fn = grf->getFilename(i);
	strcpy(__grf_getfilename, fn.c_str());
	return(__grf_getfilename);
}

unsigned int GRF_getCount(const GRF* grf) {
	return(grf->getCount());
}

} /* namespace ro */
