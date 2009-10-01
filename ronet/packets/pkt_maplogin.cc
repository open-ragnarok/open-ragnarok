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

#include "ronet/packets/pkt_maplogin.h"

namespace ronet {

pktMapLogin::pktMapLogin() :
	Packet(pktMapLoginID) {
}

pktMapLogin::pktMapLogin(int id1, int id2,
		int id3, unsigned int tick, char s) :
	Packet(pktMapLoginID) {
	account_id = id1;
	char_id = id2;
	login_id1 = id3;
	client_tick = tick;
	gender = s;
	setSize(19);
}

pktMapLogin23::pktMapLogin23() : pktMapLogin() {
	id = pktMapLogin23ID;
}

pktMapLogin23::pktMapLogin23(int id1, int id2, int id3, unsigned int tick, char s) : pktMapLogin(id1, id2, id3, tick, s) {
	id = pktMapLogin23ID;
}


bool pktMapLogin::PrepareData() {
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

bool pktMapLogin::Decode(ucBuffer& buf) {
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

void pktMapLogin::setGender(char gender) {
	this->gender = gender;
}

void pktMapLogin::setSession_id1(int session_id1) {
	this->account_id = session_id1;
}

void pktMapLogin::setSession_id2(int session_id2) {
	this->char_id = session_id2;
}

void pktMapLogin::setSession_id3(int session_id3) {
	this->login_id1 = session_id3;
}

void pktMapLogin::setTick(unsigned int tick) {
	this->client_tick = tick;
}

}
