#include "stdafx.h"

#include "ronet/packets/pkt_charlist.h"

ronet::pktCharCreated::pktCharCreated() : Packet(pktCharCreatedID) {
}

bool ronet::pktCharCreated::Decode(ronet::ucBuffer & buf) {
	if (!CheckID(buf))
		return(false);

	if (buf.dataSize() < 108) // Not enough data
		return(false);

	buf.ignore(2);

	buf >> m_char.id;
	buf >> m_char.base_xp;
	buf >> m_char.zeny;
	buf >> m_char.job_xp;
	buf >> m_char.job_lv;
	buf.ignore(sizeof(unsigned int) * 2); // opt1 + opt2
	buf >> m_char.option;
	buf >> m_char.karma;
	buf >> m_char.manner;
	buf >> m_char.status_point;
	buf >> m_char.hp;
	buf >> m_char.max_hp;
	buf >> m_char.sp;
	buf >> m_char.max_sp;
	buf >> m_char.speed;
	buf >> m_char.Class;
	buf >> m_char.hair;
	buf >> m_char.weapon;
	buf >> m_char.base_lv;
	buf >> m_char.skill_point;
	buf >> m_char.head_bottom;
	buf >> m_char.shield;
	buf >> m_char.head_top;
	buf >> m_char.head_mid;
	buf >> m_char.head_color;
	buf >> m_char.clothes_color;
	buf.read((unsigned char*)m_char.name, 24);
	buf >> m_char.attributes.Str;
	buf >> m_char.attributes.Agi;
	buf >> m_char.attributes.Vit;
	buf >> m_char.attributes.Int;
	buf >> m_char.attributes.Dex;
	buf >> m_char.attributes.Luk;
	buf >> m_char.slot;
	if (buf.peek((unsigned char*)&(m_char.rename), 2)) {
		if (m_char.rename == 1)
			buf.ignore(2);
	}

	return(true);
}