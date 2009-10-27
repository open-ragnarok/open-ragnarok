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
#include "ronet/packets/pkt_mapchange.h"

/*
0000 | 91 00 6e 65 77 5f 31 2d  32 2e 67 61 74 00 00 00 | ..new_1- 2.gat...
0010 | 00 00 64 00 09 00                                | ..d...
*/
namespace ronet {

pktMapChange::pktMapChange() : Packet(pktMapChangeID) {
	m_map[0] = 0;
	pos_x = pos_y = 0;
	setSize(22);
}

bool pktMapChange::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	// Packet size is 12...
	if (buf.dataSize() < 22)
		return(false);

	buf.ignore(2); // id
	buf.read((unsigned char*)m_map, 16);
	buf >> pos_x;
	buf >> pos_y;

	return(true);
}

void pktMapChange::getPos(int* x, int* y) const {
	*x = pos_x;
	*y = pos_y;
}

int pktMapChange::getPosX() const {
	return(pos_x);
}

int pktMapChange::getPosY() const {
	return(pos_y);
}

const char* pktMapChange::getMapName() const {
	return(m_map);
}

}
