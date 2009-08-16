#include "stdafx.h"

#include "ronet/packets/pkt_charselect.h"

ronet::pktCharSelect::pktCharSelect(unsigned int slot) : Packet(pktCharSelectID) {
	char_selected = slot;
	setSize(3);
}

bool ronet::pktCharSelect::PrepareData() {
	unsigned char* ptr = buffer;
	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);
	*ptr++ = char_selected;
	return(true);
}

bool ronet::pktCharSelect::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	if (buf.dataSize() < 3)
		return(false);

	buf >> char_selected;

	return(true);
}
