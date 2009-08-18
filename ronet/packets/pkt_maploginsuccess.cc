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
	buf >> pos_x;
	buf >> pos_y;
	buf >> pos_dir;
	buf.ignore(2);
	return (true);
}

short pktMapLoginSuccess::getPosX() const {
	return pos_x;
}

short pktMapLoginSuccess::getPosY() const {
	return pos_y;
}

unsigned char pktMapLoginSuccess::getPosDir() const {
	return pos_dir;
}

unsigned int pktMapLoginSuccess::getServerTick() const {
	return server_tick;
}

}
