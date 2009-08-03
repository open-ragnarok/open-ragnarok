/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_charcreateerror.h"

ronet::pktCharCreateError::pktCharCreateError() : Packet(pktCharCreateErrorID) {
}

const char& ronet::pktCharCreateError::getId() const {
	return(errid);
}

bool ronet::pktCharCreateError::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);

	if (buf.dataSize() < 3) // Not enough data
		return(false);
	buf.ignore(2);
	buf >> errid;

	return(true);
}
