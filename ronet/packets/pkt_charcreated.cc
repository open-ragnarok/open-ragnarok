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

#include "ronet/packets/pkt_charcreated.h"

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

	buf.clear();

	return(true);
}

CharInformation ronet::pktCharCreated::getChar(){
	return m_char;
}
