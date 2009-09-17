#include "stdafx.h"

#include "ronet/packets/pkt_displaystat.h"

ronet::pktDisplayStat::pktDisplayStat() : Packet(pktDisplayStatID) {
}

bool ronet::pktDisplayStat::Decode(ucBuffer& buf) {
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
	buf >> base;
	buf >> bonus;

	return(true);
}

unsigned int ronet::pktDisplayStat::getType(){
	return type;
}

unsigned int ronet::pktDisplayStat::getBase(){
	return base;
}

unsigned int ronet::pktDisplayStat::getBonus(){
	return bonus;
}