/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_charposition.h"

ronet::pktCharPosition::pktCharPosition() : Packet(pktCharPositionID) {
}

bool ronet::pktCharPosition::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);

	if (buf.dataSize() < 30) // Not enough data
		return(false);
	buf.ignore(2);
	buf >> position;
	buf.read((unsigned char*)mapname, 16);
	buf >> ip;
	buf >> port;

	return(true);
}

unsigned int ronet::pktCharPosition::getIp() const {
	return ip;
}

const char* ronet::pktCharPosition::getMapname() const {
	return mapname;
}

unsigned short ronet::pktCharPosition::getPort() const {
	return port;
}

unsigned int ronet::pktCharPosition::getPosition() const {
	return position;
}
