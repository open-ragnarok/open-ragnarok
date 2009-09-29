/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
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
	for (int i = 0; i < servercount; i++) {
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

