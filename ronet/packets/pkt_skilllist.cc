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

#include "ronet/packets/pkt_skilllist.h"

ronet::pktSkillList::pktSkillList() : Packet(pktSkillListID) {
}

bool ronet::pktSkillList::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	short nskill;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);
	buf >> len_list;
	buf.ignore(2);

	//Calculate number of skills received
	nskill = len_list / 37;

	//Debug info
	printf("Received %d skills.\n",nskill);

	//Read skill data
	for(int i = 0; i < nskill; i++){
		buf >> p_skills[i].id;
		buf >> p_skills[i].target_type;
		buf >> p_skills[i].unknown;
		buf >> p_skills[i].level;
		buf >> p_skills[i].sp;
		buf >> p_skills[i].range;
		buf.read((unsigned char*)p_skills[i].name, 24);
		buf >> p_skills[i].up;
	}

	buf.ignore(6);
	return(true);
}