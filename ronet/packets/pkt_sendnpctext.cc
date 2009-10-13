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

#include "ronet/packets/pkt_sendnpctext.h"

namespace ronet {

pktSendNpcText::pktSendNpcText(unsigned int npc, std::string message) : Packet(pktSendNpcTextID) {
	text = message;
	this->npc = npc;
	setSize(9 + text.length());
}


bool pktSendNpcText::PrepareData() {
	unsigned char* ptr = buffer;
	unsigned short size;

	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);

	size = 9 + text.length();

	memcpy(ptr, (unsigned char*)&size, sizeof(short));
	ptr += sizeof(short);

	memcpy(ptr, (unsigned char*)&npc, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, text.c_str(), text.length());
	ptr += text.length();
	*ptr = 0;

	return(true);
}

}
