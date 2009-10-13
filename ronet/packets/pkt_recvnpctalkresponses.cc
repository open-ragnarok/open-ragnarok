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

#include "ronet/packets/pkt_recvnpctalkresponses.h"

namespace ronet {

pktRecvNpcTalkResponses::pktRecvNpcTalkResponses() : Packet(pktRecvNpcTalkResponsesID) {
	message = NULL;
}

pktRecvNpcTalkResponses::~pktRecvNpcTalkResponses() {
	if (message != NULL)
		delete[] message;
}

void pktRecvNpcTalkResponses::DecodeMessage() {
	char line[256];
	char *start = message;
	char *ptr = message;

	lines.clear();

	while (*start != 0) {
		while (*ptr != ':' && *ptr != 0) {
			ptr++;
		}
		memset(line, 0, 256);
		strncpy(line, start, (int)(ptr - start));
		lines.push_back(line);

		if (*ptr == 0)
			break;
		start = ptr + 1;
		ptr++;
	}
}

bool pktRecvNpcTalkResponses::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	unsigned short size;
	unsigned short msgsize;
	size = *(unsigned short*)(buf.getBuffer() + 2);

	if (buf.dataSize() < size) // Not enough data
		return(false);

	buf.ignore(2); // ID
	buf.ignore(2); // Size
	buf >> npc;
	msgsize = size-8;
	message = new char[msgsize+1];
	memset(message, 0, msgsize+1);
	buf.read((unsigned char*)message, msgsize);
	DecodeMessage();

	return(true);
}

const char* pktRecvNpcTalkResponses::getMessage() const {
	return(message);
}

unsigned int pktRecvNpcTalkResponses::getNPC() const {
	return(npc);
}

unsigned int pktRecvNpcTalkResponses::getLineCount() const {
	return(lines.size());
}

const std::string& pktRecvNpcTalkResponses::getLine(unsigned int i) const {
	return(lines[i]);
}


}
