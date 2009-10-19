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

#include "ronet/packets/pkt_actorspawn.h"

namespace ronet {

pktActorSpawn::pktActorSpawn() : Packet(pktActorSpawnID) {}

pktActorSpawn::pktActorSpawn(unsigned short pktID) : Packet(pktID) {}


bool pktActorSpawn::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);
	if (buf.dataSize() < 54)
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
	//buf >> act;
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

void pktActorSpawn::Dump() {
	_log(RONET__DEBUG, "Dumping pktActorDisplay");
	_log(RONET__DEBUG, "\tID: %d", id);
	_log(RONET__DEBUG, "\tWalk speed: %d", walk_speed);
	_log(RONET__DEBUG, "\t: %d", opt1);
	_log(RONET__DEBUG, "\t: %d", opt2);
	_log(RONET__DEBUG, "\t: %d", option);
	_log(RONET__DEBUG, "\tType: %d", type);
	_log(RONET__DEBUG, "\tHair: %d", hair_style);
	_log(RONET__DEBUG, "\t: %d", weapon);
	_log(RONET__DEBUG, "\t: %d", lowhead);
	_log(RONET__DEBUG, "\t: %d", shield);
	_log(RONET__DEBUG, "\t: %d", tophead);
	_log(RONET__DEBUG, "\t: %d", midhead);
	_log(RONET__DEBUG, "\t: %d", hair_color);
	_log(RONET__DEBUG, "\t: %d", clothes_color);
	_log(RONET__DEBUG, "\tHead Direction: %d", head_dir);
	_log(RONET__DEBUG, "\tGuild: %d", guildID);
	_log(RONET__DEBUG, "\tEmblem: %d", emblemID);
	_log(RONET__DEBUG, "\t: %d", manner);
	_log(RONET__DEBUG, "\t: %d", opt3);
	_log(RONET__DEBUG, "\t: %d", karma);
	_log(RONET__DEBUG, "\tSex: %d", sex);
	_log(RONET__DEBUG, "\tCoordinates: %dx%d, %d", coord_x, coord_y, dir);
	_log(RONET__DEBUG, "\t: %d", unknown1);
	_log(RONET__DEBUG, "\t: %d", unknown2);
	//_log(RONET__DEBUG, "\tAct: %d", act);
	_log(RONET__DEBUG, "\tLevel: %d", lv);
}

}
