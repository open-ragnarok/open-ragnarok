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

#include "ro/ro.h"

namespace ro {

#define VERSION_MAJOR 0
#define VERSION_MINOR 7
#define VERSION_S ((VERSION_MAJOR << 8) | VERSION_MINOR)

char __strVer[32];

unsigned short Version() {
	return(VERSION_S);
}

void printVersion() {
	printf("0x%04x", Version());
}

const char* strVersion() {
	//sprintf(__strVer, "%d", VERSION_S);
	sprintf(__strVer, "%d.%d.svn", VERSION_MAJOR, VERSION_MINOR);
	return(__strVer);
}

} /* namespace ro */
