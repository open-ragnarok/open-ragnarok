/* $Id: pkt_login.cc 55 2009-03-01 13:01:04Z sergio $ */
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
