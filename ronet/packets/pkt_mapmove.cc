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

#include "ronet/packets/pkt_mapmove.h"

namespace ronet {

pktMapMove::pktMapMove() : Packet(pktMapMoveID) {
	m_padding = false;
	setSize(5);
}

pktMapMove::pktMapMove(int x, int y, bool p, int type) : Packet(pktMapMoveID) {
	m_padding = p;

	int size = 5;

	if (m_padding)
		size++;

	if (type == 1)
		size += 3;

	setSize(size);


	setPos(x, y);
}

bool pktMapMove::PrepareData() {
	unsigned char* ptr = buffer;
	ptr += 2; // id

	int data = 0;
	// Encode X and Y
	// Packet is like this: 8500 xxxxxxxxxxyyyyyyyyyy0000 (2+3 bytes)
	data |= ((pos_x & 0x03FF) << 14);
	data |= ((pos_y & 0x03FF) << 4);
	//printf("Encoding position: %d,%d\n", pos_x, pos_y);
	//printf("Data: 0x%08x\n", data);
	unsigned char *buf = (unsigned char*)&data;

	if (m_padding) {
		ptr[0] = 0x44;
		ptr++;
	}
	// Use only last 3 bytes
	ptr[0] = buf[2];
	ptr[1] = buf[1];
	ptr[2] = buf[0];
	return(true);
}

void pktMapMove::setPos(int x, int y) {
	pos_x = x;
	pos_y = y;
}

bool pktMapMove::Decode(ucBuffer& buf) {
	return (false);
}

}
