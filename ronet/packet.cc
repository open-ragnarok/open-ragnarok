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

#include "ronet/packet.h"

ronet::Packet::Packet() : DynamicBlob<unsigned char>() {
	id = 0;
	setSize(2);
}

ronet::Packet::Packet(unsigned short pktid) : DynamicBlob<unsigned char>() {
	id = pktid;
	setSize(2);
}

ronet::Packet::~Packet() {
}

void ronet::Packet::setSize(const unsigned long& size) {
	DynamicBlob<unsigned char>::setSize(size);
	memset(buffer, 0, dataSize);
}

bool ronet::Packet::PrepareData() {
	return(true);
}

ronet::Packet& ronet::Packet::operator >> (ronet::ucBuffer& b) {
	memcpy(buffer, (unsigned char*)&id, sizeof(short));

	if (!PrepareData())
		return(*this);

	if (blobSize() > 0)
		b.write(buffer, blobSize());
	return(*this);
}

void ronet::Packet::Dump() {
	if (!PrepareData()) {
		_log(RONET__ERROR, "Packet::Dump(): Error preparing data");
		return;
	}

	_hexlog(RONET__TRACE, buffer, dataSize);
}

bool ronet::Packet::Decode(ucBuffer&) {
	return(false);
}

unsigned short ronet::Packet::getID() const {
	return(id);
}

bool ronet::Packet::CheckID(const ucBuffer& buf) const {
	if (buf.dataSize() < 2)
		return(false);

	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		_log(RONET__ERROR, "Wrong packet id. Received: 0x%04x. Expected: 0x%04x.", id, buf_id);
		return(false);
	}

	return(true);
}
