#include "stdafx.h"

#include "ronet/packets/pkt_updatestatus.h"

ronet::pktUpdateStatus::pktUpdateStatus() : Packet(pktUpdateStatusID) {
}

bool ronet::pktUpdateStatus::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);

	//Get data
	buf >> type;
	buf >> value;

	//if (buf.dataSize() != len_mes)
	//	return(false);

	//buf.read((unsigned char*)text,len_mes-4);

	return(true);
}

unsigned short ronet::pktUpdateStatus::getType(){
	return type;
}

unsigned int ronet::pktUpdateStatus::getValue(){
	return value;
}