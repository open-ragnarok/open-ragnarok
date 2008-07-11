/* $id$ */
#include "stdafx.h"
#include "act.h"
#include "ro.h"

RO::ACT::ACT() : Object() {
	magicSize = 2;
	acts = NULL;
}

RO::ACT::~ACT() {
	if (acts != NULL) {
		for (unsigned int i = 0; i < actCount; i++) {
			if (acts[i].pat != NULL) {
				delete[] acts[i].pat;
				acts[i].pat = NULL;
			}
		}

		delete[] acts;
		acts = NULL;
	}
}

bool RO::ACT::readStream(std::istream &s) {
	if (!readHeader(s)) {
		// TODO: Raise error
		return(false);
	}

	if (!checkHeader(ACT_HEADER)) {
		std::cout << "Invalid ACT header (" << magic[0] << magic[1] << ")" << std::endl;
		return(false);
	}

	s.read((char*)&actCount, sizeof(unsigned short));
	s.read((char*)&unk1, sizeof(short));
	s.read((char*)&unk2, sizeof(int));
	s.read((char*)&unk3, sizeof(int));

	acts = new Act[actCount];
	memset(acts, 0, sizeof(Act) * actCount);

	unsigned short i;
	for (i = 0; i < actCount; i++) {
		// printf("act No. %u\n", i);

		readAct(s, &acts[i]);
	}

	// printf("Version: %d.%d\n", major_ver, minor_ver);
	return(true);

}

void RO::ACT::readAct(std::istream& s, Act* act) {
	unsigned int i;
	unsigned int p;

	s.read((char*)&act->patnum, sizeof(unsigned int));

	// One "ACT" has "act->patnum" "ActPat"s.
	// Each action has many patterns
	// A pattern is a frame in an action.
	act->pat = new Pat[act->patnum];
	memset(act->pat, 0, sizeof(Pat) * act->patnum);
	
	Pat* pat;
	for (p = 0; p < act->patnum; p++) {
		pat = &act->pat[p];

		s.read((char*)pat->pal, sizeof(unsigned int) * 2);
		s.read((char*)pat->unk, sizeof(unsigned short) * 12);
		s.read((char*)&pat->numspr, sizeof(unsigned int));

		// Each pattern may have any number of SPRITES. They are drawn on top of each
		// other in the order that is read from the file.
		pat->spr = new Spr[pat->numspr];
		memset(pat->spr, 0, sizeof(Spr) * pat->numspr);
		for (i = 0; i < pat->numspr; i++) {
			readSpr(s, &pat->spr[i]);
		}

		if (m_version.cver.major < 2)
			continue;
		
		// Sound? -- Research!
		s.read((char*)&pat->sndNo, sizeof(int));

		if (m_version.cver.minor <= 1)
			continue;
		// version greater then 0x0201
		s.read((char*)&pat->numxxx, sizeof(int));

		if (pat->numxxx > 0)
			s.read((char*)&pat->ext1, sizeof(int) * 4);
	}
}

void RO::ACT::readSpr(std::istream& s, Spr* spr) {
	s.read((char*)&spr->x, sizeof(int) * 4);
	/*
	spr->x = readInt(s);
	spr->y = readInt(s);
	spr->sprNo = readInt(s);
	spr->mirrorOn = readInt(s);
	*/

	if (m_version.cver.major <= 1)
		return;
	s.read((char*)&spr->color, sizeof(int));
	//spr->color = readInt(s);

	if (m_version.cver.minor <= 3) {
		s.read((char*)&spr->xyMag, sizeof(float));
		//spr->xyMag = readFloat(s);
	}
	else {
		s.read((char*)&spr->xMag, sizeof(float) * 2);
		//spr->xMag = readFloat(s);
		//spr->yMag = readFloat(s);
	}
	s.read((char*)&spr->rot, sizeof(int) * 2);
	//spr->rot = readInt(s);
	//spr->type = readInt(s);

	if (m_version.cver.minor <= 4)
		return;

	s.read((char*)&spr->w, sizeof(int) * 2);
	//spr->w = readInt(s);
	//spr->h = readInt(s);
}

const RO::ACT::Act* RO::ACT::operator[] (const unsigned int& i) const {
	if (i >= actCount)
		return(NULL);
	return(&acts[i]);
}

unsigned int RO::ACT::count() const {
	return(actCount);
}

void RO::ACT::Dump(std::ostream& o) const {
	o << "Version " << (short)m_version.cver.major << "." << (short)m_version.cver.minor << std::endl;
	o << "Action count: " << actCount << std::endl;
	o << "Unknowns: " << unk1 << ", " << unk2  << ", " << unk3 << std::endl;
	unsigned int i_act, i_spr, i_pat;
	for (i_act = 0; i_act < actCount; i_act++) {
		o << "Action " << i_act << std::endl;
		for (i_pat = 0; i_pat < acts[i_act].patnum ; i_pat++) {
			o << "\tACT ======" << std::endl;
			o << "\t   PAL " << acts[i_act].pat[i_pat].pal[0] << ", " << acts[i_act].pat[i_pat].pal[1] << std::endl;
			o << "\t   UNK " << acts[i_act].pat[i_pat].unk[0] << ", " << acts[i_act].pat[i_pat].unk[1] << ", " << acts[i_act].pat[i_pat].unk[2] << std::endl;
			o << "\t       " << acts[i_act].pat[i_pat].unk[3] << ", " << acts[i_act].pat[i_pat].unk[4] << ", " << acts[i_act].pat[i_pat].unk[5] << std::endl;
			o << "\t       " << acts[i_act].pat[i_pat].unk[4] << ", " << acts[i_act].pat[i_pat].unk[5] << ", " << acts[i_act].pat[i_pat].unk[6] << std::endl;
			o << "\t       " << acts[i_act].pat[i_pat].unk[7] << ", " << acts[i_act].pat[i_pat].unk[8] << ", " << acts[i_act].pat[i_pat].unk[9] << std::endl;
			o << "\t       " << acts[i_act].pat[i_pat].unk[10] << ", " << acts[i_act].pat[i_pat].unk[11] << ", " << acts[i_act].pat[i_pat].unk[12] << std::endl;
			o << "\t   SND " << acts[i_act].pat[i_pat].sndNo << std::endl;
			o << "\t   XXX " << acts[i_act].pat[i_pat].numxxx << std::endl;

			if (acts[i_act].pat[i_act].numxxx > 0)
				o << "\t   EXT " << acts[i_act].pat[i_pat].ext1 << " (" << acts[i_act].pat[i_pat].ext_x << ", " << acts[i_act].pat[i_pat].ext_y << ")" << std::endl;

			for (i_spr = 0; i_spr < acts[i_act].pat[i_pat].numspr; i_spr++) {
				o << "\t\tSPR ===" << std::endl;
				o << "\t\t    Coord ("		<< acts[i_act].pat[i_pat].spr[i_spr].x			<< ", "
											<< acts[i_act].pat[i_pat].spr[i_spr].y			<< ")" << std::endl;
				o << "\t\t    sprNo "		<< acts[i_act].pat[i_pat].spr[i_spr].sprNo		<< std::endl;
				o << "\t\t    mirrorOn "	<< acts[i_act].pat[i_pat].spr[i_spr].mirrorOn	<< std::endl;
				o << "\t\t    color "		<< acts[i_act].pat[i_pat].spr[i_spr].color		<< std::endl;
				o << "\t\t    xyMag "		<< acts[i_act].pat[i_pat].spr[i_spr].xyMag		<< std::endl;
				o << "\t\t    Mag ("		<< acts[i_act].pat[i_pat].spr[i_spr].xMag		<< ", "
											<< acts[i_act].pat[i_pat].spr[i_spr].yMag		<< ")" << std::endl;
				o << "\t\t    Rot "			<< acts[i_act].pat[i_pat].spr[i_spr].rot		<< std::endl;
				o << "\t\t    Type "		<< acts[i_act].pat[i_pat].spr[i_spr].type		<< std::endl;
				o << "\t\t    Dimensions ("	<< acts[i_act].pat[i_pat].spr[i_spr].w			<< ", "
											<< acts[i_act].pat[i_pat].spr[i_spr].h			<< ")" << std::endl;
			}
		}
	}
}

