/* $Id$ */
#include "stdafx.h"
#include "act.h"
#include "ro.h"

RO::ACT::ACT() : Object() {
	magicSize = 2;
}

RO::ACT::ACT(const ACT& a) : Object(a) {
	magicSize = 2;

	unsigned int i;
	unsigned int n = a.acts.size();
	for (i = 0; i < n; i++) {
		acts.push_back(a.acts[i]);
	}
}

void RO::ACT::ClearAll() {
	acts.clear();
}

RO::ACT& RO::ACT::operator = (const ACT& a) {
	ClearAll();
	a.copyHeader(this);

	unsigned int i;
	unsigned int n = a.acts.size();
	for (i = 0; i < n; i++) {
		acts.push_back(a.acts[i]);
	}

	return(*this);
}

RO::ACT::~ACT() {
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

	unsigned short i, n;

	s.read((char*)&n, sizeof(unsigned short));
	s.read((char*)&unk1, sizeof(short));
	s.read((char*)&unk2, sizeof(int));
	s.read((char*)&unk3, sizeof(int));

	for (i = 0; i < n; i++) {
		Act a;
		a.readStream(s, m_version);
		acts.push_back(a);
	}

	return(true);
}

bool RO::ACT::writeStream(std::ostream& o) const {
	writeHeader(o);
	unsigned short n = acts.size();
	o.write((char*)&n, sizeof(unsigned short));
	o.write((char*)&unk1, sizeof(short));
	o.write((char*)&unk2, sizeof(int));
	o.write((char*)&unk3, sizeof(int));

	unsigned short i;
	for (i = 0; i < n; i++) {
		acts[i].writeStream(o, m_version);
	}

	return(true);
}

const RO::ACT::Act& RO::ACT::operator[] (const unsigned int& i) const {
	if (i > acts.size())
		return(acts[0]);
	return(acts[i]);
}

RO::ACT::Act& RO::ACT::operator[] (const unsigned int& i) {
	if (i > acts.size())
		return(acts[0]);
	return(acts[i]);
}

const RO::ACT::Act& RO::ACT::getAct(const unsigned int& i) const {
	if (i > acts.size())
		return(acts[0]);
	return(acts[i]);
}

RO::ACT::Act& RO::ACT::getAct(const unsigned int& i) {
	if (i > acts.size())
		return(acts[0]);
	return(acts[i]);
}

unsigned int RO::ACT::count() const {
	return(acts.size());
}

void RO::ACT::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "Version " << (short)m_version.cver.major << "." << (short)m_version.cver.minor << std::endl;
	o << pfx << "Action count: " << acts.size() << std::endl;
	o << pfx << "Unknowns: " << unk1 << ", " << unk2  << ", " << unk3 << std::endl;
	unsigned int i_act, i_spr, i_pat;
	for (i_act = 0; i_act < acts.size(); i_act++) {
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
TiXmlElement *RO::ACT::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("ACT");
	char buf[32];
	sprintf(buf,"%d.%d", m_version.cver.major, m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	unsigned int i;
	unsigned int n = acts.size();
	for (i = 0; i < n; i++) {
		root->LinkEndChild(acts[i].GenerateXML(m_version));
	}

	return(root);
}
#endif

RO::ACT::Spr::Spr() {
	x = y = 0;
	sprNo = 0;
	mirrorOn = 0;
	color = 0;
	xyMag = xMag = yMag = 1;
	rot = 0;
	type = 0;
	w = h = 0;
}

RO::ACT::Spr::Spr(const Spr& s) {
	copyFrom(s);
}

RO::ACT::Spr::~Spr() {
}

void RO::ACT::Spr::copyFrom(const Spr& s) {
	x = s.x;
	y = s.y;
	sprNo = s.sprNo;
	mirrorOn = s.mirrorOn;
	color = s.color;
	xyMag = s.xyMag;
	xMag = s.xMag;
	yMag = s.yMag;
	rot = s.rot;
	type = s.type;
	w = s.w;
	h = s.h;
}

RO::ACT::Spr& RO::ACT::Spr::operator = (const Spr& s) {
	copyFrom(s);
	return(*this);
}


bool RO::ACT::Spr::readStream(std::istream& s, const s_obj_ver& v) {
	s.read((char*)&x, sizeof(int));
	s.read((char*)&y, sizeof(int));
	s.read((char*)&sprNo, sizeof(int));
	s.read((char*)&mirrorOn, sizeof(int));

	if (v.cver.major <= 1)
		return(true);

	s.read((char*)&color, sizeof(int));

	if (v.cver.minor <= 3) {
		s.read((char*)&xyMag, sizeof(float));
		xMag = xyMag;
		yMag = xyMag;
	}
	else {
		s.read((char*)&xMag, sizeof(float));
		s.read((char*)&yMag, sizeof(float));
	}

	s.read((char*)&rot, sizeof(int));
	s.read((char*)&type, sizeof(int));

	if (v.cver.minor <= 4)
		return(true);

	s.read((char*)&w, sizeof(int));
	s.read((char*)&h, sizeof(int));
	return(true);
}

bool RO::ACT::Spr::writeStream(std::ostream& s, const s_obj_ver& v) const {
	s.write((char*)&x, sizeof(int));
	s.write((char*)&y, sizeof(int));
	s.write((char*)&sprNo, sizeof(int));
	s.write((char*)&mirrorOn, sizeof(int));

	if (v.cver.major <= 1)
		return(true);

	s.write((char*)&color, sizeof(int));

	if (v.cver.minor <= 3) {
		s.write((char*)&xyMag, sizeof(float));
	}
	else {
		s.write((char*)&xMag, sizeof(float));
		s.write((char*)&yMag, sizeof(float));
	}

	s.write((char*)&rot, sizeof(int));
	s.write((char*)&type, sizeof(int));

	if (v.cver.minor <= 4)
		return(true);

	s.write((char*)&w, sizeof(int));
	s.write((char*)&h, sizeof(int));
	return(true);
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::Spr::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *e = new TiXmlElement("spr");

	char buf[32];

	e->SetAttribute("x", x);
	e->SetAttribute("y", y);
	e->SetAttribute("spr", sprNo);
	e->SetAttribute("mirror", mirrorOn);

	if (v.cver.major >= 2) {
		sprintf(buf, "#%08x", color);
		e->SetAttribute("color", buf);
		if (v.cver.minor <= 3) {
			sprintf(buf, "%f", xyMag);
			e->SetAttribute("xyMag", buf);
		}
		else {
			sprintf(buf, "%f", xMag);
			e->SetAttribute("xMag", buf);
			sprintf(buf, "%f", yMag);
			e->SetAttribute("yMag", buf);
		}

		e->SetAttribute("rot", rot);
		e->SetAttribute("type", type);
		if (v.cver.minor >= 5) {
			e->SetAttribute("w", w);
			e->SetAttribute("h", h);
		}
	}
	return(e);
}
#endif


RO::ACT::Pat::Pat() {
}

RO::ACT::Pat::Pat(const Pat& p) {
	copyFrom(p);
}

RO::ACT::Pat::~Pat() {
}

bool RO::ACT::Pat::readStream(std::istream& s, const s_obj_ver& v) {
	s.read((char*)pal, sizeof(unsigned int) * 2);
	s.read((char*)unk, sizeof(unsigned short) * 12);
	unsigned int n, i;

	s.read((char*)&n, sizeof(unsigned int));
	//std::cout << "<<SPR Count " << n << std::endl;
	numspr = n; // TODO: Remove this... no need for this.

	for (i = 0; i < n; i++) {
		Spr _s;
		_s.readStream(s, v);
		spr.push_back(_s);
	}
	//std::cout << "  Seq size: " << spr.size() << std::endl;

	if (v.cver.major < 2)
		return(true);
	
	s.read((char*)&sndNo, sizeof(int));

	if (v.cver.minor <= 1)
		return(true);

	// version greater then 0x0201
	s.read((char*)&numxxx, sizeof(int));

	if (numxxx > 0) {
		s.read((char*)&ext1, sizeof(int));
		s.read((char*)&ext_x, sizeof(int));
		s.read((char*)&ext_y, sizeof(int));
		s.read((char*)&terminate, sizeof(int));
	}


	return(true);
}

bool RO::ACT::Pat::writeStream(std::ostream& s, const s_obj_ver& v) const {
	s.write((char*)pal, sizeof(unsigned int) * 2);
	s.write((char*)unk, sizeof(unsigned short) * 12);
	
	unsigned int n = (unsigned int)spr.size();
	//std::cout << ">>SPR Count " << n << std::endl;

	s.write((char*)&n, sizeof(unsigned int));

	for (unsigned int i = 0; i < n; i++)
		spr[i].writeStream(s, v);

	if (v.cver.major < 2)
		return(true);
	
	s.write((char*)&sndNo, sizeof(int));

	if (v.cver.minor <= 1)
		return(true);

	// version greater then 0x0201
	s.write((char*)&numxxx, sizeof(int));

	if (numxxx > 0) {
		s.write((char*)&ext1, sizeof(int));
		s.write((char*)&ext_x, sizeof(int));
		s.write((char*)&ext_y, sizeof(int));
		s.write((char*)&terminate, sizeof(int));
	}


	return(true);
}

RO::ACT::Spr& RO::ACT::Pat::operator[] (unsigned int i) {
	return(spr[i]);
}

const RO::ACT::Spr& RO::ACT::Pat::operator[] (unsigned int i) const {
	return(spr[i]);
}

RO::ACT::Pat& RO::ACT::Pat::operator = (const Pat& p) {
	copyFrom(p);
	return(*this);
}

void RO::ACT::Pat::copyFrom(const Pat& p) {
	unsigned int i, n;

	pal[0] = p.pal[0];
	pal[1] = p.pal[1];

	for (i = 0; i < 12; i++)
		unk[i] = p.unk[i];

	sndNo = p.sndNo; // only in version 2.
	numxxx = p.numxxx; // version > 0x0201 (does not exists on 0x0201)
	ext1 = p.ext1;
	ext_x = p.ext_x;
	ext_y = p.ext_y;
	terminate = p.terminate;


	n = p.spr.size();
	for (i = 0; i < n; i++) {
		Spr s;
		s = p.spr[i];
		spr.push_back(s);
	}
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::Pat::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *_pat = new TiXmlElement("pat");
	TiXmlElement *e;

	unsigned int k;
	char buf[32];

	e = new TiXmlElement("pal");
	sprintf(buf,"#%08x,#%08x", pal[0], pal[1]);
	e->LinkEndChild(new TiXmlText(buf));
	_pat->LinkEndChild(e);
			
	e = new TiXmlElement("unk");
	_pat->LinkEndChild(e);
	char c = 'a';
	for (k = 0; k < 12; k++) {
		sprintf(buf, "%c", c++);
		e->SetAttribute(buf, unk[k]);
	}

	for (k = 0; k < spr.size(); k++) {
		_pat->LinkEndChild(spr[k].GenerateXML(v));
	}

	if (v.cver.major >= 2) {
		e = new TiXmlElement("sound");
		e->SetAttribute("id", sndNo);
		_pat->LinkEndChild(e);

		if (numxxx == 1) {
			e = new TiXmlElement("ext");
			_pat->LinkEndChild(e);
			e->SetAttribute("x", ext_x);
			e->SetAttribute("y", ext_y);
			sprintf(buf, "%x", terminate);
			e->SetAttribute("terminate", buf);
			sprintf(buf, "%d", ext1);
			e->LinkEndChild(new TiXmlText(buf));
		}

	}

	return(_pat);
}
#endif

RO::ACT::Act::Act() {
}

RO::ACT::Act::Act(const Act& a) {
	copyFrom(a);
}

RO::ACT::Act::~Act() {
}

bool RO::ACT::Act::readStream(std::istream& s, const s_obj_ver& v) {
	unsigned int i, n;

	s.read((char*)&n, sizeof(unsigned int));

	// One "ACT" has "n" "ActPat"s.
	// Each action has many patterns
	// A pattern is a frame in an action.

	for (i = 0; i < n; i++) {
		Pat _pat;
		_pat.readStream(s,v);
		pat.push_back(_pat);
	}

	return(true);
}

bool RO::ACT::Act::writeStream(std::ostream& s, const s_obj_ver& v) const {
	unsigned int n = pat.size();
	s.write((char*)&n, sizeof(unsigned int));
	for (unsigned int i = 0; i < n; i++) {
		pat[i].writeStream(s, v);
	}

	return(true);
}

RO::ACT::Pat& RO::ACT::Act::operator[] (unsigned int i) { return(pat[i]); }
const RO::ACT::Pat& RO::ACT::Act::operator[] (unsigned int i) const { return(pat[i]); }

RO::ACT::Act& RO::ACT::Act::operator = (const Act& a) {
	copyFrom(a);
	return(*this);
}

void RO::ACT::Act::copyFrom(const Act& a) {
	unsigned int i, n;
	n = a.pat.size();
	for (i = 0; i < n; i++) {
		Pat p;
		p = a.pat[i];
		pat.push_back(p);
	}
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::Act::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *_act = new TiXmlElement("act");
	unsigned int i, n;
	n = pat.size();

	for (i = 0; i < n; i++)
		_act->LinkEndChild(pat[i].GenerateXML(v));

	return(_act);
}
#endif