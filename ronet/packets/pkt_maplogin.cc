/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_maplogin.h"

ronet::pktMapLogin::pktMapLogin() :
	Packet(pktMapLoginID) {
}

ronet::pktMapLogin::pktMapLogin(int id1, int id2,
		int id3, unsigned int tick, char s) :
	Packet(pktMapLoginID) {
	account_id = id1;
	char_id = id2;
	login_id1 = id3;
	client_tick = tick;
	gender = s;
	setSize(19);
}

bool ronet::pktMapLogin::PrepareData() {
	unsigned char* ptr = buffer;

	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);

	memcpy(ptr, (unsigned char*)&account_id, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, (unsigned char*)&char_id, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, (unsigned char*)&login_id1, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, (unsigned char*)&client_tick, sizeof(unsigned int));
	ptr += sizeof(unsigned int);

	*ptr = gender;
	return(true);
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

	if (buf.dataSize() < 19) // Not enough data
		return (false);
	buf.ignore(2);
	buf >> account_id;
	buf >> char_id;
	buf >> login_id1;
	buf >> client_tick;
	buf >> gender;

	return (true);
}

void ronet::pktMapLogin::setGender(char gender) {
	this->gender = gender;
}

void ronet::pktMapLogin::setSession_id1(int session_id1) {
	this->account_id = session_id1;
}

void ronet::pktMapLogin::setSession_id2(int session_id2) {
	this->char_id = session_id2;
}

void ronet::pktMapLogin::setSession_id3(int session_id3) {
	this->login_id1 = session_id3;
}

void ronet::pktMapLogin::setTick(unsigned int tick) {
	this->client_tick = tick;
}
