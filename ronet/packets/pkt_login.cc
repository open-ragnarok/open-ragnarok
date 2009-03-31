/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_login.h"

ronet::pktLogin::pktLogin() : Packet(pktLoginID) {
	version = 0x14000000;
	version2 = 1;
	setSize(55);
}

ronet::pktLogin::pktLogin(const std::string& user, const std::string& pass) : Packet(pktLoginID) {
	version = 0x14000000;
	version2 = 1;
	setSize(55);

	username = user;
	password = pass;
}


bool ronet::pktLogin::PrepareData() {
	unsigned char* ptr = buffer;
	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);

	memcpy(ptr, (unsigned char*)&version, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, username.c_str(), username.length());
	ptr += 24;

	memcpy(ptr, password.c_str(), password.length());
	ptr += 24;

	memcpy(ptr, &version2, 1);

	return(true);
}
