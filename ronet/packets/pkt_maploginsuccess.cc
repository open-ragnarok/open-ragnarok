/* $Id$ */
/*
 * pkt_maploginsuccess.cc
 *
 *  Created on: 03/08/2009
 */

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
	buf >> unk1;
	buf >> unk2;
	buf >> pos1;
	buf >> pos2;
	buf >> pos3;

	return (true);
}

char pktMapLoginSuccess::getPos1() const {
	return pos1;
}

char pktMapLoginSuccess::getPos2() const {
	return pos2;
}

char pktMapLoginSuccess::getPos3() const {
	return pos3;
}

unsigned int pktMapLoginSuccess::getUnk1() const {
	return unk1;
}

unsigned short pktMapLoginSuccess::getUnk2() const {
	return unk2;
}

}
