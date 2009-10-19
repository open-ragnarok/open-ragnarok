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

#include "ronet/packets/pkt_actorwalking.h"

namespace ronet {

pktActorWalking::pktActorWalking() : pktActorSpawn(pktActorWalkingID) {
}

bool pktActorWalking::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);
	if (buf.dataSize() < 58)
		return(false);

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

	buf >> info.id;
	buf >> info.walk_speed;
	buf >> info.opt1;
	buf >> info.opt2;
	buf >> info.option;
	buf >> info.type;
	buf >> info.hair_style;
	buf >> info.weapon;
	buf >> info.lowhead;
	buf >> info.ticks; // This is the only difference from the pktActorWalking
	buf >> info.shield;
	buf >> info.tophead;
	buf >> info.midhead;
	buf >> info.hair_color;
	buf >> info.clothes_color;
	buf >> info.head_dir;
	buf >> info.guildID;
	buf >> info.emblemID;
	buf >> info.manner;
	buf >> info.opt3;
	buf >> info.karma;
	buf >> info.sex;

	buf >> coord[2];
	buf >> coord[1];
	buf >> coord[0];

	buf >> info.unknown1;
	buf >> info.unknown2;
	buf >> info.lv;

	info.dir = c & 0x0f;
	c >>= 4;
	info.coord_y = c & 0x03FF;
	c >>= 10;
	info.coord_x = c & 0x03FF;

	return(true);
}

}
