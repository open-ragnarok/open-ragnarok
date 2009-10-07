/* $Id: pkt_servertick.cc 147 2009-09-30 12:13:45Z sergio $ */
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

#include "ronet/packets/pkt_recvnpctalk.h"

namespace ronet {

pktRecvNpcTalk::pktRecvNpcTalk() : Packet(pktRecvNpcTalkID) {
	message = NULL;
}

pktRecvNpcTalk::~pktRecvNpcTalk() {
	if (message != NULL)
		delete[] message;
}

bool pktRecvNpcTalk::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);

	if (buf.dataSize() < size) // Not enough data
		return(false);

	buf.ignore(2);
	buf >> id;
	message = new char[size-4];
	memset(message, 0, size-4);
	buf.read((unsigned char*)message, size - 4);

	return(true);
}

const char* pktRecvNpcTalk::getMessage() const {
	return(message);
}

unsigned int pktRecvNpcTalk::getID() const {
	return(id);
}

}
