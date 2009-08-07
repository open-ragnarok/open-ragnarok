//[kR105]
#include "stdafx.h"

#include "ronet/packets/pkt_authfailed.h"

ronet::pktAuthFailed::pktAuthFailed() : Packet(pktAuthFailedID) {
	errorId = 0;
}

ronet::pktAuthFailed::~pktAuthFailed() {
	// D:
}

void ronet::pktAuthFailed::Dump() {
	ronet::Packet::Dump();
}

bool ronet::pktAuthFailed::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	if (buf.dataSize() < 3) // Not enough data
		return(false);

	buf.ignore(2);
	buf.peek((unsigned char*)&errorId, 1);

	// When we're done...
	buf.clear();
	return(true);
}

short ronet::pktAuthFailed::getErrorId() const {
	return(errorId);
}
