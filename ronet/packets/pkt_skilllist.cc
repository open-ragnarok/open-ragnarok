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