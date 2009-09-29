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

#include "ronet/packets/pkt_charcreate.h"

ronet::pktCharCreate::pktCharCreate() : Packet(pktCharCreateID) {
	setSize(37);
}

ronet::pktCharCreate::pktCharCreate(const std::string& name, const CharAttributes& a, unsigned char slot, unsigned short color, unsigned short style)  : Packet(pktCharCreateID) {
	setSize(37);
	attributes.Str = a.Str;
	attributes.Agi = a.Agi;
	attributes.Vit = a.Vit;
	attributes.Int = a.Int;
	attributes.Dex = a.Dex;
	attributes.Luk = a.Luk;

	charname = name;
	Slot = slot;
	this->color = color;
	this->style = style;
}

bool ronet::pktCharCreate::PrepareData() {
	unsigned char* ptr = buffer;
	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);

	memcpy(ptr, charname.c_str(), charname.length());
	ptr += 24;

	*ptr++ = attributes.Str;
	*ptr++ = attributes.Agi;
	*ptr++ = attributes.Vit;
	*ptr++ = attributes.Int;
	*ptr++ = attributes.Dex;
	*ptr++ = attributes.Luk;
	*ptr++ = Slot;

	memcpy(ptr, (char*)&color, sizeof(unsigned short));
	ptr += 2;
	memcpy(ptr, (char*)&style, sizeof(unsigned short));

	return(true);
}
