#include "stdafx.h"

#include "ronet/packets/pkt_attackrange.h"

ronet::pktAttackRange::pktAttackRange() : Packet(pktAttackRangeID) {
}

bool ronet::pktAttackRange::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);

	//Get data
	buf >> range;

	return(true);
}

unsigned short ronet::pktAttackRange::getRange(){
	return range;
}
