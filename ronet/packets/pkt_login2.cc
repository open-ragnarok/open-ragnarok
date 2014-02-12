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

#include "ronet/packets/pkt_login2.h"

ronet::pktLogin2::pktLogin2() : Packet(pktLogin2ID) {
	//version = 0x14000000;
	clienttype = 1;
	setSize(85);
}

ronet::pktLogin2::pktLogin2(const std::string& user, const std::string& pass, unsigned int version, const std::string& ip, const std::string& adapter, bool isGravity) : Packet(pktLogin2ID) {
	this->version = version;
	this->clienttype = 1;
	setSize(85);

	username = user;
	password = pass;
	ipaddr = ip;
	adapteraddr = adapter;

	isGravityID = isGravity;
}


bool ronet::pktLogin2::PrepareData() {
	unsigned char* ptr = buffer;
	ptr += sizeof(short); // ID Handled by the caller

	memcpy(ptr, (unsigned char*)&version, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, username.c_str(), username.length());
	ptr += 24;

	memcpy(ptr, password.c_str(), password.length());
	ptr += 24;

	memcpy(ptr, &clienttype, 1);
	ptr += 1;

	memcpy(ptr, ipaddr.c_str(), ipaddr.length());
	ptr += 16;

	memcpy(ptr, adapteraddr.c_str(), adapteraddr.length());
	ptr += 13;

	memcpy(ptr, &isGravityID, 1);

	return(true);
}
