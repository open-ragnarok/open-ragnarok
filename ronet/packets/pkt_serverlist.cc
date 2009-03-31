/* $Id$ */
#include "stdafx.h"

#include "ronet/packets/pkt_serverlist.h"

ronet::pktServerList::pktServerList() : Packet(pktServerListID) {
	servers = NULL;
	servercount = 0;
}

ronet::pktServerList::~pktServerList() {
	if (servers != NULL)
		delete[] servers;
}

void ronet::pktServerList::Dump() {
	ronet::Packet::Dump();
}

bool ronet::pktServerList::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);
	std::cout << "Packet size: " << size << std::endl;

	if (buf.dataSize() < size) // Not enough data
		return(false);
	
	buf.ignore(4);
	buf >> sessionid1;
	buf >> account_id;
	buf >> sessionid2;
	buf.ignore(30);
	buf >> sex;

	servercount = (size - 47)/32;
	
	servers = new ServerInfo[servercount];
	for (unsigned int i = 0; i < servercount; i++) {
		buf >> servers[i].ip;
		buf >> servers[i].port;
		buf.read((unsigned char*)servers[i].name, 20);
		buf >> servers[i].users;
		buf >> servers[i].maintenance;
		buf >> servers[i].New;
	}

	// When we're done...
	//buf.ignore(size);
	return(true);
}

const ronet::pktServerList::ServerInfo& ronet::pktServerList::getInfo(const int& i) const {
	return(servers[i]);
}

unsigned int ronet::pktServerList::getServerCount() const {
	return(servercount);
}

unsigned int ronet::pktServerList::getAccountId() const {
	return(account_id);
}

unsigned int ronet::pktServerList::getSessionId1() const {
	return(sessionid1);
}

unsigned int ronet::pktServerList::getSessionId2() const {
	return(sessionid2);
}

unsigned char ronet::pktServerList::getSex() const {
	return(sex);
}

