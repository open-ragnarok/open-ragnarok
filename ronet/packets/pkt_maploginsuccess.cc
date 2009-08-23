/* $Id$ */
/*
 * pkt_maploginsuccess.cc
 *
 *  Created on: 03/08/2009
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
