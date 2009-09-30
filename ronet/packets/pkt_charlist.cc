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

	if (buf.dataSize() < size)//(size + 7)) // Not enough data
		return(false);
	
	m_count = (size - 24) / 108;
	m_chars = new CharInformation[m_count];

	buf.ignore(4);
	buf.ignore(20); // unknowns
	// Read DATA
	for (int i = 0; i < m_count; i++) {
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
		//buf.ignore(1);
	}

	//buf.ignore(7);
	buf.clear();
	return(true);
}

int ronet::pktCharList::getCount() const {
	return(m_count);
}

const CharInformation& ronet::pktCharList::getChar(int i) const {
	return(m_chars[i]);
}
