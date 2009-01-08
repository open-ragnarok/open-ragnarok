/* $Id$ */
#include "stdafx.h"
#include "act.h"
#include "ro.h"

RO::ACT::ACT() : Object() {
	magicSize = 2;
	acts = NULL;
}

RO::ACT::ACT(const ACT& a) : Object(a) {
	magicSize = 2;
	actCount = a.actCount;
	if (actCount > 1000)
		actCount = 0;

	if (actCount > 0)
		acts = new Act[actCount];

	unk1 = a.unk1;
	unk2 = a.unk2;
	unk3 = a.unk3;
	
	for (unsigned int i = 0; i < actCount; i++) {
		acts[i].patnum = a.acts[i].patnum;
		acts[i].pat = new Pat[acts[i].patnum];
		for(unsigned int j = 0; j < acts[i].patnum; j++) {
			acts[i].pat[j].pal[0] = a.acts[i].pat[j].pal[0];
			acts[i].pat[j].pal[1] = a.acts[i].pat[j].pal[1];
			for (int x = 0; x < 12; x++)
				acts[i].pat[j].unk[x] = a.acts[i].pat[j].unk[x];

			acts[i].pat[j].numspr = a.acts[i].pat[j].numspr;
			acts[i].pat[j].spr = NULL;
			if (acts[i].pat[j].numspr > 0) {
				acts[i].pat[j].spr = new Spr[acts[i].pat[j].numspr];
				for (unsigned int k = 0; k < acts[i].pat[j].numspr; k++) {
					acts[i].pat[j].spr[k].x = a.acts[i].pat[j].spr[k].x;
					acts[i].pat[j].spr[k].y = a.acts[i].pat[j].spr[k].y;
					acts[i].pat[j].spr[k].sprNo = a.acts[i].pat[j].spr[k].sprNo;
					acts[i].pat[j].spr[k].mirrorOn = a.acts[i].pat[j].spr[k].mirrorOn;
					acts[i].pat[j].spr[k].color = a.acts[i].pat[j].spr[k].color;
					acts[i].pat[j].spr[k].xyMag = a.acts[i].pat[j].spr[k].xyMag;
					acts[i].pat[j].spr[k].xMag = a.acts[i].pat[j].spr[k].xMag;
					acts[i].pat[j].spr[k].yMag = a.acts[i].pat[j].spr[k].yMag;
					acts[i].pat[j].spr[k].rot = a.acts[i].pat[j].spr[k].rot;
					acts[i].pat[j].spr[k].type = a.acts[i].pat[j].spr[k].type;
					acts[i].pat[j].spr[k].w = a.acts[i].pat[j].spr[k].w;
					acts[i].pat[j].spr[k].h = a.acts[i].pat[j].spr[k].h;
				}
			}
			acts[i].pat[j].sndNo = a.acts[i].pat[j].sndNo;
			acts[i].pat[j].numxxx = a.acts[i].pat[j].numxxx;
			acts[i].pat[j].ext1 = a.acts[i].pat[j].ext1;
			acts[i].pat[j].ext_x = a.acts[i].pat[j].ext_x;
			acts[i].pat[j].ext_y = a.acts[i].pat[j].ext_y;
			acts[i].pat[j].terminate = a.acts[i].pat[j].terminate;
		}
	}
}

RO::ACT& RO::ACT::operator = (const ACT& a) {
	if (acts != NULL) {
		for (unsigned int i = 0; i < actCount; i++) {
			if (acts[i].pat != NULL) {
				for (unsigned int j = 0; j < acts[i].patnum; j++)
					delete[] acts[i].pat[j].spr;

				delete[] acts[i].pat;
			}
		}

		delete[] acts;
		acts = NULL;
	}

	a.copyHeader(this);
	magicSize = 2;
	actCount = a.actCount;

	unk1 = a.unk1;
	unk2 = a.unk2;
	unk3 = a.unk3;

	if (actCount > 1000)
		actCount = 0;

	if (actCount > 0)
		acts = new Act[actCount];
	else {
		return(*this);
	}

	for (unsigned int i = 0; i < actCount; i++) {
		acts[i].patnum = a.acts[i].patnum;
		acts[i].pat = new Pat[acts[i].patnum];
		for(unsigned int j = 0; j < acts[i].patnum; j++) {
			acts[i].pat[j].pal[0] = a.acts[i].pat[j].pal[0];
			acts[i].pat[j].pal[1] = a.acts[i].pat[j].pal[1];
			for (int x = 0; x < 12; x++)
				acts[i].pat[j].unk[x] = a.acts[i].pat[j].unk[x];

			acts[i].pat[j].numspr = a.acts[i].pat[j].numspr;
			acts[i].pat[j].spr = NULL;
			if (acts[i].pat[j].numspr > 0) {
				acts[i].pat[j].spr = new Spr[acts[i].pat[j].numspr];
				for (unsigned int k = 0; k < acts[i].pat[j].numspr; k++) {
					acts[i].pat[j].spr[k].x = a.acts[i].pat[j].spr[k].x;
					acts[i].pat[j].spr[k].y = a.acts[i].pat[j].spr[k].y;
					acts[i].pat[j].spr[k].sprNo = a.acts[i].pat[j].spr[k].sprNo;
					acts[i].pat[j].spr[k].mirrorOn = a.acts[i].pat[j].spr[k].mirrorOn;
					acts[i].pat[j].spr[k].color = a.acts[i].pat[j].spr[k].color;
					acts[i].pat[j].spr[k].xyMag = a.acts[i].pat[j].spr[k].xyMag;
					acts[i].pat[j].spr[k].xMag = a.acts[i].pat[j].spr[k].xMag;
					acts[i].pat[j].spr[k].yMag = a.acts[i].pat[j].spr[k].yMag;
					acts[i].pat[j].spr[k].rot = a.acts[i].pat[j].spr[k].rot;
					acts[i].pat[j].spr[k].type = a.acts[i].pat[j].spr[k].type;
					acts[i].pat[j].spr[k].w = a.acts[i].pat[j].spr[k].w;
					acts[i].pat[j].spr[k].h = a.acts[i].pat[j].spr[k].h;
				}
			}
			acts[i].pat[j].sndNo = a.acts[i].pat[j].sndNo;
			acts[i].pat[j].numxxx = a.acts[i].pat[j].numxxx;
			acts[i].pat[j].ext1 = a.acts[i].pat[j].ext1;
			acts[i].pat[j].ext_x = a.acts[i].pat[j].ext_x;
			acts[i].pat[j].ext_y = a.acts[i].pat[j].ext_y;
			acts[i].pat[j].terminate = a.acts[i].pat[j].terminate;
		}
	}

	return(*this);
}

RO::ACT::~ACT() {
	if (acts != NULL) {
		for (unsigned int i = 0; i < actCount; i++) {
			if (acts[i].pat != NULL) {
				for (unsigned int j = 0; j < acts[i].patnum; j++)
					delete[] acts[i].pat[j].spr;

				delete[] acts[i].pat;
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

void RO::ACT::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "Version " << (short)m_version.cver.major << "." << (short)m_version.cver.minor << std::endl;
	o << pfx << "Action count: " << actCount << std::endl;
	o << pfx << "Unknowns: " << unk1 << ", " << unk2  << ", " << unk3 << std::endl;
	unsigned int i_act, i_spr, i_pat;
	for (i_act = 0; i_act < actCount; i_act++) {
		o << pfx << "Action " << i_act << std::endl;
		for (i_pat = 0; i_pat < acts[i_act].patnum ; i_pat++) {
			o << pfx << "\tACT ======" << std::endl;
			o << pfx << "\t   PAL " << acts[i_act].pat[i_pat].pal[0] << ", " << acts[i_act].pat[i_pat].pal[1] << std::endl;
			o << pfx << "\t   UNK " << acts[i_act].pat[i_pat].unk[0] << ", " << acts[i_act].pat[i_pat].unk[1] << ", " << acts[i_act].pat[i_pat].unk[2] << std::endl;
			o << pfx << "\t       " << acts[i_act].pat[i_pat].unk[3] << ", " << acts[i_act].pat[i_pat].unk[4] << ", " << acts[i_act].pat[i_pat].unk[5] << std::endl;
			o << pfx << "\t       " << acts[i_act].pat[i_pat].unk[4] << ", " << acts[i_act].pat[i_pat].unk[5] << ", " << acts[i_act].pat[i_pat].unk[6] << std::endl;
			o << pfx << "\t       " << acts[i_act].pat[i_pat].unk[7] << ", " << acts[i_act].pat[i_pat].unk[8] << ", " << acts[i_act].pat[i_pat].unk[9] << std::endl;
			o << pfx << "\t       " << acts[i_act].pat[i_pat].unk[10] << ", " << acts[i_act].pat[i_pat].unk[11] << ", " << acts[i_act].pat[i_pat].unk[12] << std::endl;
			o << pfx << "\t   SND " << acts[i_act].pat[i_pat].sndNo << pfx << std::endl;
			o << pfx << "\t   XXX " << acts[i_act].pat[i_pat].numxxx << std::endl;

			if (acts[i_act].pat[i_act].numxxx > 0)
				o << pfx << "\t   EXT " << acts[i_act].pat[i_pat].ext1 << " (" << acts[i_act].pat[i_pat].ext_x << ", " << acts[i_act].pat[i_pat].ext_y << ")" << std::endl;

			for (i_spr = 0; i_spr < acts[i_act].pat[i_pat].numspr; i_spr++) {
				o << pfx << "\t\tSPR ===" << std::endl;
				o << pfx << "\t\t    Coord ("		<< acts[i_act].pat[i_pat].spr[i_spr].x			<< ", "
													<< acts[i_act].pat[i_pat].spr[i_spr].y			<< ")" << std::endl;
				o << pfx << "\t\t    sprNo "		<< acts[i_act].pat[i_pat].spr[i_spr].sprNo		<< std::endl;
				o << pfx << "\t\t    mirrorOn "		<< acts[i_act].pat[i_pat].spr[i_spr].mirrorOn	<< std::endl;
				o << pfx << "\t\t    color "		<< acts[i_act].pat[i_pat].spr[i_spr].color		<< std::endl;
				o << pfx << "\t\t    xyMag "		<< acts[i_act].pat[i_pat].spr[i_spr].xyMag		<< std::endl;
				o << pfx << "\t\t    Mag ("			<< acts[i_act].pat[i_pat].spr[i_spr].xMag		<< ", "
													<< acts[i_act].pat[i_pat].spr[i_spr].yMag		<< ")" << std::endl;
				o << pfx << "\t\t    Rot "			<< acts[i_act].pat[i_pat].spr[i_spr].rot		<< std::endl;
				o << pfx << "\t\t    Type "			<< acts[i_act].pat[i_pat].spr[i_spr].type		<< std::endl;
				o << pfx << "\t\t    Dimensions ("	<< acts[i_act].pat[i_pat].spr[i_spr].w			<< ", "
													<< acts[i_act].pat[i_pat].spr[i_spr].h			<< ")" << std::endl;
			}
		}
	}
}

#ifdef ROINT_USE_XML

TiXmlElement *RO::ACT::GenerateXML(const Spr& s) const {
	TiXmlElement *e = new TiXmlElement("spr");

	char buf[32];

	e->SetAttribute("x", s.x);
	e->SetAttribute("y", s.y);
	e->SetAttribute("spr", s.sprNo);
	e->SetAttribute("mirror", s.mirrorOn);

	if (m_version.cver.major >= 2) {
		sprintf(buf, "#%08x", s.color);
		e->SetAttribute("color", buf);
		if (m_version.cver.minor <= 3) {
			sprintf(buf, "%f", s.xyMag);
			e->SetAttribute("xyMag", buf);
		}
		else {
			sprintf(buf, "%f", s.xMag);
			e->SetAttribute("xMag", buf);
			sprintf(buf, "%f", s.yMag);
			e->SetAttribute("yMag", buf);
		}

		e->SetAttribute("rot", s.rot);
		e->SetAttribute("type", s.type);
		if (m_version.cver.minor >= 5) {
			e->SetAttribute("w", s.w);
			e->SetAttribute("h", s.h);
		}
	}
	return(e);
}

TiXmlElement *RO::ACT::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("ACT");
	char buf[32];
	sprintf(buf,"%d.%d", m_version.cver.major, m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	TiXmlElement *e;
	std::string s;

	unsigned int i, j, k;

	// acts
	for(i = 0 ; i < actCount; i++) {
		TiXmlElement *_act = new TiXmlElement("act");
		root->LinkEndChild(_act);
		for (j = 0; j < acts[i].patnum; j++) {
			TiXmlElement *_pat = new TiXmlElement("pat");
			_act->LinkEndChild(_pat);
			e = new TiXmlElement("pal");
			sprintf(buf,"#%08x,#%08x", acts[i].pat[j].pal[0], acts[i].pat[j].pal[1]);
			e->LinkEndChild(new TiXmlText(buf));
			_pat->LinkEndChild(e);
			
			e = new TiXmlElement("unk");
			_pat->LinkEndChild(e);
			char c = 'a';
			for (k = 0; k < 12; k++) {
				sprintf(buf, "%c", c++);
				e->SetAttribute(buf, acts[i].pat[j].unk[k]);
				//sprintf(buf, "%d", acts[i].pat[j].unk[k]);
				//e->LinkEndChild(new TiXmlText(buf));
			}

			for (k = 0; k < acts[i].pat[j].numspr; k++) {
				_pat->LinkEndChild(GenerateXML(acts[i].pat[j].spr[k]));
			}

			if (m_version.cver.major >= 2) {
				e = new TiXmlElement("sound");
				e->SetAttribute("id", acts[i].pat[j].sndNo);
				_pat->LinkEndChild(e);

				if (acts[i].pat[j].numxxx == 1) {
					e = new TiXmlElement("ext");
					_pat->LinkEndChild(e);
					e->SetAttribute("x", acts[i].pat[j].ext_x);
					e->SetAttribute("y", acts[i].pat[j].ext_y);
					sprintf(buf, "%x", acts[i].pat[j].terminate);
					e->SetAttribute("terminate", buf);
					sprintf(buf, "%d", acts[i].pat[j].ext1);
					e->LinkEndChild(new TiXmlText(buf));
				}
			}
		}
	}

	return(root);
}

TiXmlDocument RO::ACT::GenerateXMLDoc(const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateXML(name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}
bool RO::ACT::SaveXML(std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	out << doc;
	return(true);
}
bool RO::ACT::SaveXML(const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	doc.SaveFile(fn);
	return(true);
}
#endif
