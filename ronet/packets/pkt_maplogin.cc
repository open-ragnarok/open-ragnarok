/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_maplogin.h"

ronet::pktMapLogin::pktMapLogin() :
	Packet(pktMapLoginID) {
}

ronet::pktMapLogin::pktMapLogin(unsigned int id1, unsigned int id2,
		unsigned int id3, unsigned int tick, char s) :
	Packet(pktMapLoginID) {
	session_id1 = id1;
	session_id2 = id2;
	session_id3 = id3;
	this->tick = tick;
	gender = s;
}

bool ronet::pktMapLogin::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	buf.peek((unsigned char*) &buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return (false);
	}
	unsigned short size;
	size = *(unsigned short*) (buf.getBuffer() + 2);

	if (buf.dataSize() < 3) // Not enough data
		return (false);
	buf.ignore(2);
	buf >> session_id1;
	buf >> session_id2;
	buf >> session_id3;
	buf >> tick;
	buf >> gender;

	return (true);
}

void ronet::pktMapLogin::setGender(char gender) {
	this->gender = gender;
}

void ronet::pktMapLogin::setSession_id1(unsigned int session_id1) {
	this->session_id1 = session_id1;
}

void ronet::pktMapLogin::setSession_id2(unsigned int session_id2) {
	this->session_id2 = session_id2;
}

void ronet::pktMapLogin::setSession_id3(unsigned int session_id3) {
	this->session_id3 = session_id3;
}

void ronet::pktMapLogin::setTick(unsigned int tick) {
	this->tick = tick;
}
