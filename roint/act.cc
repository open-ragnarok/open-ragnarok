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
#include "ro/types/act.h"
#include "ro/ro.h"

static RO::ACT::Action g_emptyAction;
static RO::ACT::Motion g_emptyMotion;
static RO::ACT::SprClip g_emptySprClip;

RO::ACT::ACT() : Object() {
	magicSize = 2;
	m_version.sver = 0x205;

	memset(m_reserved, 0, sizeof(m_reserved));
}

RO::ACT::ACT(const ACT& a) : Object(a) {
	memcpy(m_reserved, a.m_reserved, sizeof(m_reserved));
	m_actions = a.m_actions;
	m_events = a.m_events;
	m_delays = a.m_delays;
}

void RO::ACT::reset() {
	m_valid = false;
	memset(m_reserved, 0, sizeof(m_reserved));
	m_actions.clear();
	m_events.clear();
	m_delays.clear();
}

RO::ACT& RO::ACT::operator = (const ACT& a) {
	reset();
	a.copyHeader(this);

	memcpy(m_reserved, a.m_reserved, sizeof(m_reserved));
	m_actions = a.m_actions;
	m_events = a.m_events;
	m_delays = a.m_delays;
	return(*this);
}

RO::ACT::~ACT() {
	reset();
}

bool RO::ACT::readStream(std::istream &s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(ACT_HEADER)) {
		return(false);
	}

	switch (m_version.sver) {
		default:
			if (m_version.sver >= 0x100 && m_version.sver <= 0x1FF)
				break;// supported? not sure what's the base version... probably 0x100 or 0x101
			return(false);
		case 0x200: // color/zoom/angle/clipType in sprite clip
		case 0x201: // array of events
		case 0x202: // array of delays
		case 0x203: // array of attach points in motion
		case 0x204: // separate x/y zoom in sprite clip
		case 0x205: // width/height in sprite clip
			break;// supported
	}

	// read actions
	unsigned int i;
	unsigned short nActions;
	s.read((char*)&nActions, sizeof(unsigned short));
	if (s.fail())
		nActions = 0;
	s.read((char*)&m_reserved, sizeof(m_reserved));
	m_actions.resize(nActions);
	for (i = 0; i < m_actions.size(); i++) {
		Action& act = m_actions[i];
		if (!act.readStream(s, m_version)) {
			reset();
			return(false);
		}
	}
	// read events
	if (m_version.sver >= 0x201) {
		int nEvents;
		s.read((char*)&nEvents, sizeof(int));
		if (s.fail())
			nEvents = 0;
		m_events.resize(nEvents);
		for (i = 0; i < m_events.size(); i++) {
			Event evt;
			s.read((char*)&evt, sizeof(evt));
			if (s.fail())
				break;
			evt.name[39] = 0;
			m_events[i] = evt;
		}
	}
	// read delays
	if (m_version.sver >= 0x202) {
		m_delays.resize(m_actions.size());
		for (i = 0; i < m_delays.size(); i++) {
			float delay;
			s.read((char*)&delay, sizeof(float));
			if (s.fail())
				break;
			m_delays[i] = delay;
		}
	}

	if (s.fail()) {
		reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

bool RO::ACT::writeStream(std::ostream& o) const {
	if (!isValid()) {
		return(false);
	}
	if (!writeHeader(o)) {
		return(false);
	}
	unsigned short nActions = m_actions.size();
	o.write((char*)&nActions, sizeof(unsigned short));
	o.write((char*)&m_reserved, sizeof(m_reserved));

	unsigned int i;
	// write actions
	for (i = 0; i < nActions; i++) {
		if (!m_actions[i].writeStream(o, m_version))
			return(false);
	}
	// write events
	if (m_version.sver >= 0x201) {
		int nEvents = (int)m_events.size();
		o.write((char*)&nEvents, sizeof(int));
		for (i = 0; i < m_events.size(); i++) {
			const Event& evt = m_events[i];
			o.write((char*)&evt, sizeof(Event));
		}
	}
	// write delays
	if (m_version.sver >= 0x202) {
		for (i = 0; i < nActions; i++) {
			const float& delay = m_delays[i];
			o.write((char*)&delay, sizeof(float));
		}
	}

	return(!o.fail());
}

const RO::ACT::Action& RO::ACT::operator[] (unsigned int act) const {
	return(m_actions[act]);
}

unsigned int RO::ACT::getActionCount() const {
	return(m_actions.size());
}

const RO::ACT::Action& RO::ACT::getAction(unsigned int act) const {
	if (act < m_actions.size())
		return(m_actions[act]);
	return(g_emptyAction);
}

unsigned int RO::ACT::getMotionCount(unsigned int act) const {
	if (act < m_actions.size())
		return(m_actions[act].motions.size());
	return(0);
}

const RO::ACT::Motion& RO::ACT::getMotion(unsigned int act, unsigned int mot) const {
	if (act < m_actions.size())
		return(m_actions[act].getMotion(mot));
	return(g_emptyMotion);
}

const char* RO::ACT::getEventName(unsigned int evt) const {
	if (evt < m_events.size())
		return(m_events[evt].name);
	return "";
}

float RO::ACT::getDelay(unsigned int act) const {
	if (act < m_delays.size())
		return(m_delays[act]);
	return(4.0f);
}

void RO::ACT::Dump(std::ostream& o, const std::string& pfx) const {
	char buf[256];
	sprintf(buf,"%d.%d", (m_version.sver >> 8) & 0xFF, m_version.sver & 0xFF);
	o << pfx << "Version " << buf << std::endl;
	o << pfx << "Action count: " << m_actions.size() << std::endl;
	sprintf(buf, "%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x",
		m_reserved[0], m_reserved[1], m_reserved[2], m_reserved[3], m_reserved[4],
		m_reserved[5], m_reserved[6], m_reserved[7], m_reserved[8], m_reserved[9]);
	o << pfx << "Reserved: " << buf << std::endl;
	unsigned int i_act, i_spr, i_mot, i_point, i_event, i_delay;
	for (i_act = 0; i_act < m_actions.size(); i_act++) {
		const Action& act = m_actions[i_act];
		o << pfx << "Action " << i_act << ":" << std::endl;
		for (i_mot = 0; i_mot < act.motions.size() ; i_mot++) {
			const Motion& mot = act.motions[i_mot];
			o << pfx << "\tMotion " << i_mot << ":" << std::endl;
			o << pfx << "\t range1 " << mot.range1[0] << ", " << mot.range1[1] << mot.range1[2] << ", " << mot.range1[3] << std::endl;
			o << pfx << "\t range2 " << mot.range2[0] << ", " << mot.range2[1] << mot.range2[2] << ", " << mot.range2[3] << std::endl;
			o << pfx << "\t eventId " << mot.eventId << std::endl;
			o << pfx << "\t Attach point count: " << mot.attachPoints.size() << std::endl;
			for (i_point = 0; i_point < mot.attachPoints.size(); i_point++) {
				o << pfx << "\t  attach point " << i_point << ": " << mot.attachPoints[i_point].x << ", " << mot.attachPoints[i_point].y << ", " << mot.attachPoints[i_point].attr << std::endl;
			}

			o << pfx << "\t Sprite clip count: " << mot.sprClips.size() << std::endl;
			for (i_spr = 0; i_spr < mot.sprClips.size(); i_spr++) {
				const SprClip& sprClip = mot.sprClips[i_spr];
				o << pfx << "\t\tSprClip " << i_spr << ":" << std::endl;
				o << pfx << "\t\t Coord (" << sprClip.x << ", " << sprClip.y << ")" << std::endl;
				o << pfx << "\t\t sprNo " << sprClip.sprNo << std::endl;
				o << pfx << "\t\t mirrorOn " << sprClip.mirrorOn << std::endl;
				sprintf(buf, "#%08x", sprClip.color);
				o << pfx << "\t\t color " << buf << std::endl;
				o << pfx << "\t\t Mag (" << sprClip.xMag << ", " << sprClip.yMag << ")" << std::endl;
				o << pfx << "\t\t angle " << sprClip.angle << std::endl;
				o << pfx << "\t\t sprType " << sprClip.sprType << std::endl;
				o << pfx << "\t\t Dimensions (" << sprClip.w << ", " << sprClip.h << ")" << std::endl;
			}
		}
	}
	if (m_version.sver >= 0x201) {
		o << pfx << "Event count: " << m_events.size() << std::endl;
		for (i_event = 0; i_event < m_events.size(); i_event++) {
			o << pfx << " event " << i_event << ": " << m_events[i_event].name << std::endl;
		}
	}
	if (m_version.sver >= 0x202) {
		o << pfx << "Delay count: " << m_delays.size() << std::endl;
		for (i_delay = 0; i_delay < m_events.size(); i_delay++) {
			o << pfx << " delay " << i_delay << ": " << m_delays[i_delay] << std::endl;
		}
	}
}

#ifdef ROINT_USE_XML
TiXmlElement *RO::ACT::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("ACT");
	char buf[32];
	sprintf(buf,"%d.%d", (m_version.sver >> 8) & 0xFF, m_version.sver & 0xFF);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	unsigned int i, nActions, nEvents, nDelays;
	nActions = m_actions.size();
	nEvents = m_events.size();
	nDelays = m_events.size();
	for (i = 0; i < nActions; i++) {
		root->LinkEndChild(m_actions[i].GenerateXML(m_version));
	}
	for (i = 0; i < nEvents; i++) {
		TiXmlElement *e = new TiXmlElement("event");
		e->SetValue(m_events[i].name);
		root->LinkEndChild(e);
	}
	for (i = 0; i < nDelays; i++) {
		TiXmlElement *e = new TiXmlElement("delay");
		sprintf(buf, "%f", m_delays[i]);
		e->SetValue(buf);
		root->LinkEndChild(e);
	}
	return(root);
}
#endif

RO::ACT::SprClip::SprClip() {
	x = y = 0;
	sprNo = -1;
	mirrorOn = 0;
	color = 0xFFFFFFFF;
	xMag = yMag = 1;
	angle = 0;
	sprType = 0;
	w = h = 0;
}

RO::ACT::SprClip::SprClip(const SprClip& s) {
	copyFrom(s);
}

RO::ACT::SprClip::~SprClip() {
}

void RO::ACT::SprClip::copyFrom(const SprClip& s) {
	x = s.x;
	y = s.y;
	sprNo = s.sprNo;
	mirrorOn = s.mirrorOn;
	color = s.color;
	xMag = s.xMag;
	yMag = s.yMag;
	angle = s.angle;
	sprType = s.sprType;
	w = s.w;
	h = s.h;
}

RO::ACT::SprClip& RO::ACT::SprClip::operator = (const SprClip& s) {
	copyFrom(s);
	return(*this);
}


bool RO::ACT::SprClip::readStream(std::istream& s, const s_obj_ver& v) {
	s.read((char*)&x, sizeof(int));
	s.read((char*)&y, sizeof(int));
	s.read((char*)&sprNo, sizeof(int));
	s.read((char*)&mirrorOn, sizeof(int));

	color = 0xFFFFFFFF;
	xMag = yMag = 1;
	angle = 0;
	sprType = 0;
	w = h = 0;

	if (v.sver >= 0x200) {
		s.read((char*)&color, sizeof(unsigned int));
		if (v.sver >= 0x204) {
			s.read((char*)&xMag, sizeof(float));
			s.read((char*)&yMag, sizeof(float));
		}
		else {
			float xyMag;
			s.read((char*)&xyMag, sizeof(float));
			xMag = xyMag;
			yMag = xyMag;
		}
		s.read((char*)&angle, sizeof(int));
		s.read((char*)&sprType, sizeof(int));
		if (v.sver >= 0x205) {
			s.read((char*)&w, sizeof(int));
			s.read((char*)&h, sizeof(int));
		}
	}

	return(!s.fail());
}

bool RO::ACT::SprClip::writeStream(std::ostream& s, const s_obj_ver& v) const {
	s.write((char*)&x, sizeof(int));
	s.write((char*)&y, sizeof(int));
	s.write((char*)&sprNo, sizeof(int));
	s.write((char*)&mirrorOn, sizeof(int));

	if (v.sver >= 0x200) {
		s.write((char*)&color, sizeof(unsigned int));
		if (v.sver >= 0x204) {
			s.write((char*)&xMag, sizeof(float));
			s.write((char*)&yMag, sizeof(float));
		}
		else {
			s.write((char*)&xMag, sizeof(float));
		}
		s.write((char*)&angle, sizeof(int));
		s.write((char*)&sprType, sizeof(int));
		if (v.sver >= 0x205) {
			s.write((char*)&w, sizeof(int));
			s.write((char*)&h, sizeof(int));
		}
	}

	return(!s.fail());
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::SprClip::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *e = new TiXmlElement("sprclip");

	char buf[32];

	e->SetAttribute("x", x);
	e->SetAttribute("y", y);
	e->SetAttribute("spr", sprNo);
	e->SetAttribute("mirror", mirrorOn);

	if (v.sver >= 0x200) {
		sprintf(buf, "#%08x", color);
		e->SetAttribute("color", buf);
		if (v.sver >= 0x204) {
			sprintf(buf, "%f", xMag);
			e->SetAttribute("xMag", buf);
			sprintf(buf, "%f", yMag);
			e->SetAttribute("yMag", buf);
		}
		else {
			sprintf(buf, "%f", xMag);
			e->SetAttribute("xyMag", buf);
		}
		e->SetAttribute("angle", angle);
		e->SetAttribute("sprType", sprType);
		if (v.sver >= 0x205) {
			e->SetAttribute("w", w);
			e->SetAttribute("h", h);
		}
	}
	return(e);
}
#endif


RO::ACT::Motion::Motion() {
	range1[0] = 0;
	range1[1] = 0;
	range1[2] = 0;
	range1[3] = 0;
	range2[0] = 0;
	range2[1] = 0;
	range2[2] = 0;
	range2[3] = 0;
	eventId = -1;
}

RO::ACT::Motion::Motion(const Motion& p) {
	copyFrom(p);
}

RO::ACT::Motion::~Motion() {
}

bool RO::ACT::Motion::readStream(std::istream& s, const s_obj_ver& v) {
	sprClips.clear();
	attachPoints.clear();

	s.read((char*)range1, sizeof(range1));
	s.read((char*)range2, sizeof(range2));
	eventId = -1;

	int i, nSprClip;

	// read sprite clips
	s.read((char*)&nSprClip, sizeof(int));
	if (s.fail())
		return(false);
	if (nSprClip > 0)
		sprClips.resize(nSprClip);
	for (i = 0; i < nSprClip; i++) {
		SprClip& clip = sprClips[i];
		if (!clip.readStream(s, v))
			return(false);
	}
	// read eventId
	if (v.sver >= 0x200) {
		s.read((char*)&eventId, sizeof(int));
		if (v.sver == 0x200)
			eventId = -1;// TODO why does this happen? [flaviojs]
	}
	// read attach points
	if (v.sver >= 0x203) {
		int i, nAttachPoint;
		int ignored;
		s.read((char*)&nAttachPoint, sizeof(int));
		if (s.fail())
			return(false);
		attachPoints.resize(nAttachPoint);
		for (i = 0; i < nAttachPoint; i++) {
			AttachPoint& point = attachPoints[i];
			s.read((char*)&ignored, sizeof(int));
			s.read((char*)&point, sizeof(AttachPoint));
		}
	}
	return(!s.fail());
}

bool RO::ACT::Motion::writeStream(std::ostream& s, const s_obj_ver& v) const {
	s.write((char*)range1, sizeof(range1));
	s.write((char*)range2, sizeof(range2));

	// write sprites
	int i, nSprClip = (int)sprClips.size();
	s.write((char*)&nSprClip, sizeof(int));
	for (i = 0; i < nSprClip; i++) {
		const SprClip& clip = sprClips[i];
		if (!clip.writeStream(s, v))
			return(false);
	}
	// write event id
	if (v.sver >= 0x200) {
		s.write((char*)&eventId, sizeof(int));
	}
	// write attach points
	if (v.sver >= 0x203) {
		unsigned int i, n = attachPoints.size();
		int ignored = 0;
		s.write((char*)&eventId, sizeof(int));
		for (i = 0; i < n; i++) {
			const AttachPoint& point = attachPoints[i];
			s.write((char*)&ignored, sizeof(int));
			s.write((char*)&point, sizeof(AttachPoint));
		}
	}

	return(!s.fail());
}

RO::ACT::SprClip& RO::ACT::Motion::operator[] (unsigned int i) {
	return(sprClips[i]);
}

const RO::ACT::SprClip& RO::ACT::Motion::operator[] (unsigned int i) const {
	return(sprClips[i]);
}

unsigned int RO::ACT::Motion::getClipCount() const {
	return(sprClips.size());
}

const RO::ACT::SprClip& RO::ACT::Motion::getClip(unsigned int clp) const {
	if (clp < sprClips.size())
		return(sprClips[clp]);
	return(g_emptySprClip);
}

RO::ACT::Motion& RO::ACT::Motion::operator = (const Motion& p) {
	copyFrom(p);
	return(*this);
}

void RO::ACT::Motion::copyFrom(const Motion& p) {
	range1[0] = p.range1[0];
	range1[1] = p.range1[1];
	range1[2] = p.range1[2];
	range1[3] = p.range1[3];
	range2[0] = p.range2[0];
	range2[1] = p.range2[1];
	range2[2] = p.range2[2];
	range2[3] = p.range2[3];
	eventId = p.eventId;
	sprClips = p.sprClips;
	attachPoints = p.attachPoints;
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::Motion::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *_mot = new TiXmlElement("motion");
	TiXmlElement *e;

	unsigned int i;
	char buf[256];

	e = new TiXmlElement("range1");
	sprintf(buf,"%d,%d,%d,%d", range1[0], range1[1], range1[2], range1[3]);
	e->LinkEndChild(new TiXmlText(buf));
	_mot->LinkEndChild(e);

	e = new TiXmlElement("range2");
	sprintf(buf,"%d,%d,%d,%d", range2[0], range2[1], range2[2], range2[3]);
	e->LinkEndChild(new TiXmlText(buf));
	_mot->LinkEndChild(e);

	for (i = 0; i < sprClips.size(); i++) {
		_mot->LinkEndChild(sprClips[i].GenerateXML(v));
	}

	if (v.sver >= 0x200) {
		e = new TiXmlElement("event");
		e->SetAttribute("id", eventId);
		_mot->LinkEndChild(e);
	}

	if (v.sver >= 0x203) {
		for (i = 0; i < attachPoints.size(); i++) {
			e = new TiXmlElement("attachpoint");
			_mot->LinkEndChild(e);
			e->SetAttribute("x", attachPoints[i].x);
			e->SetAttribute("y", attachPoints[i].y);
			e->SetAttribute("attr", attachPoints[i].attr);
		}
	}

	return(_mot);
}
#endif

RO::ACT::Action::Action() {
}

RO::ACT::Action::Action(const Action& a) {
	copyFrom(a);
}

RO::ACT::Action::~Action() {
}

bool RO::ACT::Action::readStream(std::istream& s, const s_obj_ver& v) {
	int i, nMotions;

	motions.clear();
	s.read((char*)&nMotions, sizeof(int));
	if (s.fail())
		return(false);
	if (nMotions > 0)
		motions.resize(nMotions);
	for (i = 0; i < nMotions; i++) {
		Motion& mot = motions[i];
		if (!mot.readStream(s,v))
			return(false);
	}

	return(true);
}

bool RO::ACT::Action::writeStream(std::ostream& s, const s_obj_ver& v) const {
	int nMotion = motions.size();
	s.write((char*)&nMotion, sizeof(int));
	if (s.fail())
		return(false);
	for (int i = 0; i < nMotion; i++) {
		const Motion& mot = motions[i];
		if (!mot.writeStream(s, v))
			return(false);
	}
	return(true);
}

RO::ACT::Motion& RO::ACT::Action::operator[] (unsigned int i) { return(motions[i]); }
const RO::ACT::Motion& RO::ACT::Action::operator[] (unsigned int i) const { return(motions[i]); }

unsigned int RO::ACT::Action::getMotionCount() const {
	return(motions.size());
}

const RO::ACT::Motion& RO::ACT::Action::getMotion(unsigned int mot) const {
	if (mot < motions.size())
		return(motions[mot]);
	return(g_emptyMotion);
}

RO::ACT::Action& RO::ACT::Action::operator = (const Action& a) {
	copyFrom(a);
	return(*this);
}

void RO::ACT::Action::copyFrom(const Action& a) {
	motions = a.motions;
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::ACT::Action::GenerateXML(const s_obj_ver& v) const {
	TiXmlElement *_act = new TiXmlElement("action");
	unsigned int i, n;
	n = motions.size();

	for (i = 0; i < n; i++)
		_act->LinkEndChild(motions[i].GenerateXML(v));

	return(_act);
}
#endif
