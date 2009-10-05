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

#include "ronet/packets/pkt_actordisplay.h"

namespace ronet {

pktActorDisplay::pktActorDisplay() : Packet(pktActorDisplayID) {
}

bool pktActorDisplay::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);
	if (buf.dataSize() < 55)
		return(false);

	//_hexlog(RONET__DEBUG, buf.getBuffer(), 55);

	buf.ignore(2); // Packet ID


	unsigned int c = 0;
	unsigned char *coord = (unsigned char*)&c;

	buf >> unk;
	/* Padding, according to eAthena code:
#if PACKETVER >= 20071106
	if (type) {
		// shift payload 1 byte to the right for mob packets
		WBUFB(buf,2) = 0; // padding?
		offset++;
		buf = WBUFP(buffer,offset);
	}
#endif
	 My headache is gone!
	 */

	buf >> id;
	buf >> walk_speed;
	buf >> opt1;
	buf >> opt2;
	buf >> option;
	buf >> type;
	buf >> hair_style;
	buf >> weapon;
	buf >> lowhead;
	buf >> shield;
	buf >> tophead;
	buf >> midhead;
	buf >> hair_color;
	buf >> clothes_color;
	buf >> head_dir;
	buf >> guildID;
	buf >> emblemID;
	buf >> manner;
	buf >> opt3;
	buf >> karma;
	buf >> sex;

	buf >> coord[2];
	buf >> coord[1];
	buf >> coord[0];

	buf >> unknown1;
	buf >> unknown2;
	buf >> act;
	buf >> lv;

	//XYdecode(coord);

	dir = c & 0x0f;
	c >>= 4;
	coord_y = c & 0x03FF;
	c >>= 10;
	coord_x = c & 0x03FF;

	//_log(RONET__DEBUG, "There is someone at %d, %d (id: %08x)!", coord_x, coord_y, id);
	//_log(RONET__DEBUG, "Type: %d", type);

	return(true);
}

//by kR105
void pktActorDisplay::XYdecode(unsigned char buf[3]){
	//Decode the encoded X, Y and DIR sent by the server.
	dir = buf[2] & 0x0F;
	coord_x = (((buf[1] & 0x3F) << 4) | (buf[2] >> 4));
	coord_y = ((buf[0] << 2) | (buf[1] >> 6));
}


}
