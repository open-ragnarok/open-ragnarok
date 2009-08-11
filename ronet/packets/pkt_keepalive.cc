#include "stdafx.h"

#include "ronet/packets/pkt_keepalive.h"

ronet::pktKeepAlive::pktKeepAlive(unsigned int acct) : Packet(pktKeepAliveID) {
	account_id = acct;
	setSize(6);
}

bool ronet::pktKeepAlive::PrepareData() {
	unsigned char* ptr = buffer;
	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);
	memcpy(ptr, (unsigned char*)&account_id, sizeof(int));
	return(true);
}

bool ronet::pktKeepAlive::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	if (buf.dataSize() < 6)
		return(false);

	buf.ignore(2);
	buf >> account_id;

	return(true);
}
