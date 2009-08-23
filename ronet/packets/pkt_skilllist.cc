#include "stdafx.h"

#include "ronet/packets/pkt_skilllist.h"

ronet::pktSkillList::pktSkillList() : Packet(pktSkillListID) {
}

bool ronet::pktSkillList::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);
	buf >> len_list;
	buf.ignore(2);
	printf("Received %d skills.\n",len_list / 37);
	buf.ignore(len_list - 4);
	buf.ignore(6);
	return(true);
}