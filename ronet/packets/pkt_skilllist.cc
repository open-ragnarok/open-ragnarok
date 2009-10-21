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

namespace ronet {

pktSkillList::pktSkillList() : Packet(pktSkillListID) {
	p_skills = NULL;
}
pktSkillList::~pktSkillList() {
	if (p_skills != NULL)
		delete[] p_skills;
}

bool pktSkillList::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	short nskill;

	buf.ignore(2);
	buf >> len_list;

	//Calculate number of skills received
	nskill = (len_list - 4) / 37; // 4 = length of id + size
	p_skills = new SkillInformation[nskill];

	//Debug info
	_log(RONET__DEBUG, "Received %d skills.", nskill);

	//Read skill data
	for(int i = 0; i < nskill; i++){
		buf >> p_skills[i].id;
		buf >> p_skills[i].target_type;
		buf >> p_skills[i].level;
		buf >> p_skills[i].sp;
		buf >> p_skills[i].range;
		buf.read((unsigned char*)p_skills[i].name, 24);
		buf >> p_skills[i].up;
		_log(RONET__DEBUG, "\tSkill %d: (%s)", p_skills[i].id, p_skills[i].name);
	}

	// buf.ignore(6); //?????
	return(true);
}

}
