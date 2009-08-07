//[kR105]
#include "stdafx.h"

#include "ronet/packets/pkt_loginerror.h"

ronet::pktLoginError::pktLoginError() : Packet(pktLoginErrorID) {
	errorId = 0;
}

ronet::pktLoginError::~pktLoginError() {
	// D:
}

void ronet::pktLoginError::Dump() {
	ronet::Packet::Dump();
}

bool ronet::pktLoginError::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);

	if (buf.dataSize() < size) // Not enough data
		return(false);

	buf.ignore(2);
	buf >> errorId;

	// When we're done...
	//buf.ignore(size);
	buf.clear();
	return(true);
}

short ronet::pktLoginError::getErrorId() const {
	return(errorId);
}
