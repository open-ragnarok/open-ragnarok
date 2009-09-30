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
#include "ronet/packets/pkt_maploginsuccess.h"

namespace ronet {

pktMapLoginSuccess::pktMapLoginSuccess() :
	Packet(pktMapLoginSuccessID) {
}

bool pktMapLoginSuccess::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	unsigned char zbuf[3];

	buf.peek((unsigned char*) &buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return (false);
	}
	unsigned short size;
	size = *(unsigned short*) (buf.getBuffer() + 2);

	if (buf.dataSize() < 11) // Not enough data
		return (false);
	buf.ignore(2);
	buf >> server_tick;
	buf >> zbuf[0];
	buf >> zbuf[1];
	buf >> zbuf[2];
	XYdecode(zbuf);
	//delete[] zbuf;
	buf.ignore(2);
	return (true);
}

//by kR105
void pktMapLoginSuccess::XYdecode(unsigned char buf[3]){
	//Decode the encoded X, Y and DIR sent by the server.
	pos_dir = buf[2] & 0x0F;
	pos_y = (((buf[1] & 0x3F) << 4) | (buf[2] >> 4));
	pos_x = ((buf[0] << 2) | (buf[1] >> 6));
}

short pktMapLoginSuccess::getPosX() const {
	return pos_x;
}

short pktMapLoginSuccess::getPosY() const {
	return pos_y;
}

short pktMapLoginSuccess::getPosDir() const {
	return pos_dir;
}

unsigned int pktMapLoginSuccess::getServerTick() const {
	return server_tick;
}

}
