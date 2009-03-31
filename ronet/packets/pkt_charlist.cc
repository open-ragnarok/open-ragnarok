#include "stdafx.h"

#include "ronet/packets/pkt_charlist.h"

ronet::pktCharList::pktCharList() : Packet(pktCharListID) {
	m_count = 0;
	m_chars = NULL;
}

ronet::pktCharList::~pktCharList()  {
	if (m_chars != NULL)
		delete[] m_chars;
}

bool ronet::pktCharList::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	unsigned short size;
	size = *(unsigned short*)(buf.getBuffer() + 2);
	std::cout << "Packet size: " << size << std::endl;

	if (buf.dataSize() < (size + 7)) // Not enough data
		return(false);
	
	int charcount = (size - 24) / 108;
	m_chars = new CharInformation[charcount];

	buf.ignore(4);
	buf.ignore(20); // unknowns
	// Read DATA
	for (int i = 0; i < charcount; i++) {
		buf >> m_chars[i].id;
		buf >> m_chars[i].base_xp;
		buf >> m_chars[i].zeny;
		buf >> m_chars[i].job_xp;
		buf >> m_chars[i].job_lv;
		buf.ignore(sizeof(unsigned int) * 2); // opt1 + opt2
		buf >> m_chars[i].option;
		buf >> m_chars[i].karma;
		buf >> m_chars[i].manner;
		buf >> m_chars[i].status_point;
		buf >> m_chars[i].hp;
		buf >> m_chars[i].max_hp;
		buf >> m_chars[i].sp;
		buf >> m_chars[i].max_sp;
		buf >> m_chars[i].speed;
		buf >> m_chars[i].Class;
		buf >> m_chars[i].hair;
		buf >> m_chars[i].weapon;
		buf >> m_chars[i].base_lv;
		buf >> m_chars[i].skill_point;
		buf >> m_chars[i].head_bottom;
		buf >> m_chars[i].shield;
		buf >> m_chars[i].head_top;
		buf >> m_chars[i].head_mid;
		buf >> m_chars[i].head_color;
		buf >> m_chars[i].clothes_color;
		buf.read((unsigned char*)m_chars[i].name, 24);
		buf >> m_chars[i].attributes.Str;
		buf >> m_chars[i].attributes.Agi;
		buf >> m_chars[i].attributes.Vit;
		buf >> m_chars[i].attributes.Int;
		buf >> m_chars[i].attributes.Dex;
		buf >> m_chars[i].attributes.Luk;
		buf >> m_chars[i].slot;
		if (buf.peek((unsigned char*)&(m_chars[i].rename), 2)) {
			if (m_chars[i].rename == 1)
				buf.ignore(2);
		}
	}

	buf.ignore(7);
	return(true);
}

int ronet::pktCharList::getCount() const {
	return(m_count);
}

const CharInformation& ronet::pktCharList::getChar(int i) const {
	return(m_chars[i]);
}