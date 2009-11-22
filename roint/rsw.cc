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
#include "ro/types/rsw.h"
#include "ro/ro.h"

#include <vector>

namespace ro {

/*
RSW::OT_Model = 1;
RSW::OT_Light = 2;
RSW::OT_Sound = 3;
RSW::OT_Effect = 4;
*/

RSW::RSW() : ro::Object() {
}

RSW::RSW(const RSW& rsw) : ro::Object(rsw) {
	strcpy(m_iniFile, rsw.m_iniFile);
	strcpy(m_gndFile, rsw.m_gndFile);
	strcpy(m_gatFile, rsw.m_gatFile);
	strcpy(m_scrFile, rsw.m_scrFile);
	m_water = rsw.m_water;
	m_light = rsw.m_light;
	m_ground = rsw.m_ground;
	unsigned int i, n = rsw.m_objects.size();
	m_objects.resize(n);
	for (i = 0; i < n; i++) {
		if (rsw.m_objects[i] != NULL)
			m_objects[i] = rsw.m_objects[i]->Copy();
		else
			m_objects[i] = NULL;
	}
}

RSW::~RSW() {
	reset();
}

bool RSW::readStream(std::istream& s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(RSW_HEADER)) {
		return(false);
	}

	if (m_version.cver.major == 1 && m_version.cver.minor >= 2 && m_version.cver.minor <= 9) {
		// supported [1.2 1.9]
	}
	else if (m_version.cver.major == 2 && m_version.cver.minor <= 1) {
		// supported [2.0 2.1]
	}
	else {
		return(false);// unsupported version
	}

	// read filenames
	s.read(m_iniFile, 40);
	s.read(m_gndFile, 40);
	if (IsCompatibleWith(1, 4)) {
		s.read(m_gatFile, 40);
	}
	else {
		memset(m_gatFile, 0, 40);
	}
	s.read(m_scrFile, 40);
	m_iniFile[39] = m_gndFile[39] = m_gatFile[39] = m_scrFile[39] = 0;

	// read water
	if (IsCompatibleWith(1,3)) {
		s.read((char*)&m_water.level, sizeof(float));
	}
	else {
		m_water.level = 0.0f;
	}
	if (IsCompatibleWith(1,8)) {
		s.read((char*)&m_water.type, sizeof(int));
		s.read((char*)&m_water.waveHeight, sizeof(float));
		s.read((char*)&m_water.waveSpeed, sizeof(float));
		s.read((char*)&m_water.wavePitch, sizeof(float));
	}
	else {
		m_water.type = 0;
		m_water.waveHeight = 1.0f;
		m_water.waveSpeed = 2.0f;
		m_water.wavePitch = 50.0f;
	}
	if (IsCompatibleWith(1,9)) {
		s.read((char*)&m_water.animSpeed, sizeof(int));
	}
	else {
		m_water.animSpeed = 3;
	}

	// read light
	if (IsCompatibleWith(1,5)) {
		s.read((char*)&m_light.longitude, sizeof(int));
		s.read((char*)&m_light.latitude, sizeof(int));
		s.read((char*)&m_light.diffuse, sizeof(float) * 3);
		s.read((char*)&m_light.ambient, sizeof(float) * 3);
	}
	else {
		m_light.longitude = 45;
		m_light.latitude = 45;
		m_light.diffuse[0] = m_light.diffuse[1] = m_light.diffuse[2] = 1.0f;
		m_light.ambient[0] = m_light.ambient[1] = m_light.ambient[2] = 0.3f;
	}
	if (IsCompatibleWith(1,7)) {
		s.read((char*)&m_light.ignored, sizeof(float));
	}

	// read ground
	if (IsCompatibleWith(1,6)) {
		s.read((char*)&m_ground, sizeof(Ground));
	}
	else {
		m_ground.top = -500;
		m_ground.bottom = 500;
		m_ground.left = -500;
		m_ground.right = 500;
	}

	// read objects
	int i, nObjects;
	s.read((char*)&nObjects, sizeof(int));
	if (nObjects > 0)
		m_objects.resize((unsigned int)nObjects);
	for (i = 0; i < nObjects; i++) {
		m_objects[i] = Object::readStream(s, m_version);
		if (m_objects[i] == NULL) {
			reset();
			return(false);
		}
	}

	// read quadtree
	if (IsCompatibleWith(2,1)) {
		unsigned int i = 0;
		m_quadTree.resize(1365);// 4^0 + 4^1 + 4^2 + 4^3 + 4^4 + 4^5
		readQuadTree(s, 0, i);
	}

	if (s.fail()) {
		reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

void RSW::readQuadTree(std::istream& s, unsigned int level, unsigned int& i) {
	QuadTreeNode& node = m_quadTree[i];
	s.read((char*)&node.max, sizeof(float) * 3);
	s.read((char*)&node.min, sizeof(float) * 3);
	s.read((char*)&node.halfSize, sizeof(float) * 3);
	s.read((char*)&node.center, sizeof(float) * 3);
	i++;
	if (level < 5) {
		node.child[0] = i;
		readQuadTree(s, level + 1, i);
		node.child[1] = i;
		readQuadTree(s, level + 1, i);
		node.child[2] = i;
		readQuadTree(s, level + 1, i);
		node.child[3] = i;
		readQuadTree(s, level + 1, i);
	}
	else {
		node.child[0] = 0;
		node.child[1] = 0;
		node.child[2] = 0;
		node.child[3] = 0;
	}
}

bool RSW::writeStream(std::ostream& s) const {
	if (!isValid()) {
		return(false);
	}
	if (!writeHeader(s)) {
		return(false);
	}

	// write filenames
	s.write(m_iniFile, 40);
	s.write(m_gndFile, 40);
	if (IsCompatibleWith(1, 4)) {
		s.write(m_gatFile, 40);
	}
	s.write(m_scrFile, 40);

	// read water
	if (IsCompatibleWith(1,3)) {
		s.write((char*)&m_water.level, sizeof(float));
	}
	if (IsCompatibleWith(1,8)) {
		s.write((char*)&m_water.type, sizeof(int));
		s.write((char*)&m_water.waveHeight, sizeof(float));
		s.write((char*)&m_water.waveSpeed, sizeof(float));
		s.write((char*)&m_water.wavePitch, sizeof(float));
	}
	if (IsCompatibleWith(1,9)) {
		s.write((char*)&m_water.animSpeed, sizeof(int));
	}

	// read light
	if (IsCompatibleWith(1,5)) {
		s.write((char*)&m_light.longitude, sizeof(int));
		s.write((char*)&m_light.latitude, sizeof(int));
		s.write((char*)&m_light.diffuse, sizeof(float) * 3);
		s.write((char*)&m_light.ambient, sizeof(float) * 3);
	}
	if (IsCompatibleWith(1,7)) {
		s.write((char*)&m_light.ignored, sizeof(float));
	}

	// read ground
	if (IsCompatibleWith(1,6)) {
		s.write((char*)&m_ground, sizeof(Ground));
	}

	// read objects
	int i, nObjects = (int)m_objects.size();
	s.write((char*)&nObjects, sizeof(int));
	for (i = 0; i < nObjects; i++) {
		const Object* obj = m_objects[i];
		if (!obj->writeStream(s, m_version)) {
			return(false);
		}
	}

	// read quadtree
	if (IsCompatibleWith(2,1)) {
		unsigned int i, n = m_quadTree.size();
		for (i = 0; i < n; i++) {
			const QuadTreeNode& node = m_quadTree[i];
			s.write((char*)&node.max, sizeof(float) * 3);
			s.write((char*)&node.min, sizeof(float) * 3);
			s.write((char*)&node.halfSize, sizeof(float) * 3);
			s.write((char*)&node.center, sizeof(float) * 3);
		}
	}
	return(!s.fail());
}

void RSW::Dump(std::ostream& o, const std::string& pfx) const {
	char buf[1024];
	sprintf(buf,"Magic: %c%c%c%c", magic[0], magic[1], magic[2], magic[3]);
	o << pfx << buf << std::endl;
	sprintf(buf,"Version: %u.%u", m_version.cver.major, m_version.cver.minor);
	o << pfx << buf << std::endl;

	o << pfx << "Ini: " << m_iniFile << std::endl;
	o << pfx << "Gnd: " << m_gndFile << std::endl;
	o << pfx << "Gat: " << m_gatFile << std::endl;
	o << pfx << "Scr: " << m_scrFile << std::endl;

	sprintf(buf,"Water");
	o << pfx << buf << std::endl;
	sprintf(buf,"\tLevel: %f", m_water.level);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tType: %d", m_water.type);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tWave height: %.2f", m_water.waveHeight);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tWave speed: %.2f", m_water.waveSpeed);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tWave pitch: %.2f", m_water.wavePitch);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tAnim speed: %d", m_water.animSpeed);
	o << pfx << buf << std::endl;

	sprintf(buf,"Light");
	o << pfx << buf << std::endl;
	sprintf(buf,"\tLongitude: %d", m_light.longitude);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tLatitude: %d", m_light.latitude);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tDiffuse: %.2f %.2f %.2f", m_light.diffuse[0], m_light.diffuse[1], m_light.diffuse[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tAmbient: %.2f %.2f %.2f", m_light.ambient[0], m_light.ambient[1], m_light.ambient[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tIgnored: %.2f", m_light.ignored);
	o << pfx << buf << std::endl;

	sprintf(buf,"Ground");
	o << pfx << buf << std::endl;
	sprintf(buf,"\tTop: %d", m_ground.top);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tBottom: %d", m_ground.bottom);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tLeft: %d", m_ground.left);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tRight: %d", m_ground.right);
	o << pfx << buf << std::endl;

	sprintf(buf,"Objects: %u", m_objects.size());
	o << pfx << buf << std::endl;
	unsigned int i;
	for (i = 0; i < m_objects.size(); i++) {
		m_objects[i]->Dump(o);
	}
}

void RSW::reset() {
	m_valid = false;
	memset(m_iniFile, 0, 40);
	memset(m_gndFile, 0, 40);
	memset(m_gatFile, 0, 40);
	memset(m_scrFile, 0, 40);
	unsigned int i, n = m_objects.size();
	for (i = 0; i < n; i++) {
		delete m_objects[i];
	}
	m_objects.clear();
	m_quadTree.clear();
}

const char* RSW::getIniFile() const {
	return(this->m_iniFile);
}

const char* RSW::getGndFile() const {
	return(this->m_gndFile);
}

const char* RSW::getGatFile() const {
	return(this->m_gatFile);
}

const char* RSW::getScrFile() const {
	return(this->m_scrFile);
}

const RSW::Water& RSW::getWater() const {
	return(this->m_water);
}

const RSW::Light& RSW::getLight() const {
	return(this->m_light);
}

const RSW::Ground& RSW::getGround() const {
	return(this->m_ground);
}

unsigned int RSW::getObjectCount() const {
	return(m_objects.size());
}

const RSW::Object* RSW::getObject(unsigned int obj) const {
	return(m_objects[obj]);
}

const RSW::Object* RSW::operator[] (unsigned int obj) const {
	return(m_objects[obj]);
}

const RSW::ModelObject* RSW::getModelObject(unsigned int obj) const {
	if (m_objects[obj]->isType(ModelType))
		return((RSW::ModelObject*)m_objects[obj]);
	return(NULL);
}

const RSW::LightObject* RSW::getLightObject(unsigned int obj) const {
	if (m_objects[obj]->isType(LightType))
		return((RSW::LightObject*)m_objects[obj]);
	return(NULL);
}

const RSW::SoundObject* RSW::getSoundObject(unsigned int obj) const {
	if (m_objects[obj]->isType(SoundType))
		return((RSW::SoundObject*)m_objects[obj]);
	return(NULL);
}

const RSW::EffectObject* RSW::getEffectObject(unsigned int obj) const {
	if (m_objects[obj]->isType(EffectType))
		return((RSW::EffectObject*)m_objects[obj]);
	return(NULL);
}

bool RSW::isType(unsigned int obj, ObjectType t) const {
	if (obj < m_objects.size())
		return(m_objects[obj]->isType(t));
	return(false);
}

RSW& RSW::operator = (const RSW& rsw) {
	reset();
	rsw.copyHeader(this);
	strcpy(m_iniFile, rsw.m_iniFile);
	strcpy(m_gndFile, rsw.m_gndFile);
	strcpy(m_gatFile, rsw.m_gatFile);
	strcpy(m_scrFile, rsw.m_scrFile);
	m_water = rsw.m_water;
	m_light = rsw.m_light;
	m_ground = rsw.m_ground;
	unsigned int i, n = rsw.m_objects.size();
	m_objects.resize(n);
	for (i = 0; i < n; i++) {
		m_objects[i] = rsw.m_objects[i]->Copy();
	}
	return(*this);
}

/* === XML === */
#ifdef ROINT_USE_XML

#include <fstream>

TiXmlElement *RSW::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("RSW");

	char buf[16];
	sprintf(buf,"%d.%d", m_version.cver.major , m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") root->SetAttribute("name", name);
	// if (utf) std::cout << "Saving XML with UTF-8 Encoding" << std::endl;

	if (strlen(m_iniFile)) {
		TiXmlElement *file = new TiXmlElement("inifile");
		std::string n;
		if (utf)
			n = euc2utf8(m_iniFile);
		else
			n = m_iniFile;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(m_gndFile)) {
		TiXmlElement *file = new TiXmlElement("gndfile");
		std::string n;
		if (utf)
			n = euc2utf8(m_gndFile);
		else
			n = m_gndFile;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(m_gatFile)) {
		TiXmlElement *file = new TiXmlElement("gatfile");
		std::string n;
		if (utf)
			n = euc2utf8(m_gatFile);
		else
			n = m_gatFile;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(m_scrFile)) {
		TiXmlElement *file = new TiXmlElement("scrfile");
		std::string n;
		if (utf)
			n = euc2utf8(m_scrFile);
		else
			n = m_scrFile;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}

	// Water
	{
		TiXmlElement* attr;
		TiXmlElement* water = new TiXmlElement("water");

		sprintf(buf, "%f", this->m_water.level);
		attr = new TiXmlElement("level");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		sprintf(buf, "%d", this->m_water.type);
		attr = new TiXmlElement("type");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		sprintf(buf, "%f", this->m_water.waveHeight);
		attr = new TiXmlElement("waveheight");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		sprintf(buf, "%f", this->m_water.waveSpeed);
		attr = new TiXmlElement("wavespeed");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		sprintf(buf, "%f", this->m_water.wavePitch);
		attr = new TiXmlElement("wavepitch");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		sprintf(buf, "%d", this->m_water.animSpeed);
		attr = new TiXmlElement("animspeed");
		attr->LinkEndChild(new TiXmlText(buf));
		water->LinkEndChild(attr);

		root->LinkEndChild(water);
	}

	// Light
	{
		TiXmlElement* attr;
		TiXmlElement* light = new TiXmlElement("light");

		sprintf(buf, "%d", this->m_light.longitude);
		attr = new TiXmlElement("longitude");
		attr->LinkEndChild(new TiXmlText(buf));
		light->LinkEndChild(attr);

		sprintf(buf, "%d", this->m_light.latitude);
		attr = new TiXmlElement("latitude");
		attr->LinkEndChild(new TiXmlText(buf));
		light->LinkEndChild(attr);

		attr = new TiXmlElement("diffuse");
		sprintf(buf, "%f", this->m_light.diffuse[0]);
		attr->SetAttribute("r", buf);
		sprintf(buf, "%f", this->m_light.diffuse[1]);
		attr->SetAttribute("g", buf);
		sprintf(buf, "%f", this->m_light.diffuse[2]);
		attr->SetAttribute("b", buf);
		light->LinkEndChild(attr);

		attr = new TiXmlElement("ambient");
		sprintf(buf, "%f", this->m_light.ambient[0]);
		attr->SetAttribute("r", buf);
		sprintf(buf, "%f", this->m_light.ambient[1]);
		attr->SetAttribute("g", buf);
		sprintf(buf, "%f", this->m_light.ambient[2]);
		attr->SetAttribute("b", buf);
		light->LinkEndChild(attr);

		sprintf(buf, "%f", this->m_light.ignored);
		attr = new TiXmlElement("ignored");
		attr->LinkEndChild(new TiXmlText(buf));
		light->LinkEndChild(attr);

		root->LinkEndChild(light);
	}

	for (unsigned int i = 0; i < m_objects.size(); i++) {
		TiXmlElement* attr;
		TiXmlElement* object = new TiXmlElement("object");
		switch(m_objects[i]->getType()) {
			case ModelType:
				object->SetAttribute("type", "model");
				{
					ModelObject* mdl = (ModelObject*)m_objects[i];
					if (utf) {
						object->SetAttribute("name", euc2utf8(mdl->name));
						object->SetAttribute("modelname", euc2utf8(mdl->modelName));
						object->SetAttribute("nodename", euc2utf8(mdl->nodeName));
					}
					else {
						object->SetAttribute("name", mdl->name);
						object->SetAttribute("modelname", mdl->modelName);
						object->SetAttribute("nodename", mdl->nodeName);
					}
					object->SetAttribute("animtype", mdl->animType);
					sprintf(buf, "%f", mdl->animSpeed);
					object->SetAttribute("animspeed", buf);
					object->SetAttribute("blocktype", mdl->blockType);

					attr = new TiXmlElement("pos");
					sprintf(buf, "%f", mdl->pos[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->pos[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->pos[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);

					attr = new TiXmlElement("rot");
					sprintf(buf, "%f", mdl->rot[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->rot[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->rot[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);

					attr = new TiXmlElement("scale");
					sprintf(buf, "%f", mdl->scale[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->scale[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->scale[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);
				}
				break;
			case EffectType:
				object->SetAttribute("type", "effect");
				// TODO
				break;
			case LightType:
				object->SetAttribute("type", "light");
				// TODO
				break;
			case SoundType:
				object->SetAttribute("type", "sound");
				// TODO
				break;
			default:
				break;
		}

		root->LinkEndChild(object);
	}

	return(root);
}

TiXmlElement *RSW::GenerateFullXML(const std::map<std::string, RSM> rsm, const std::string& name, bool utf) const {
	TiXmlElement *root = GenerateXML(name, utf);

	// populate RSM
	std::vector<std::string> saved;
	std::map<std::string, RSM>::const_iterator rsm_itr;
	bool found = false;
	std::string fname;
	for (unsigned int i = 0; i < getObjectCount(); i++) {
		const RSW::ModelObject* mdl = getModelObject(i);
		if (mdl == NULL)
			continue;

		fname = mdl->modelName;
		found = false;
		for(std::vector<std::string>::const_iterator itr = saved.begin(); itr != saved.end(); itr++) {
			if (*itr == fname)
				found = true;
		}

		if (!found) {
			saved.push_back(fname);
			rsm_itr = rsm.find(fname);
			if (rsm_itr != rsm.end()) {
				TiXmlElement* e = rsm_itr->second.GenerateXML(fname, utf);
				root->LinkEndChild(e);
			}
			else {
				std::cout << "can't find file " << fname << "!" << std::endl;
			}
		}
	}

	return(root);
}


TiXmlElement *RSW::GenerateFullXML(const std::map<std::string, RSM*> rsm, const std::string& name, bool utf) const {
	TiXmlElement *root = GenerateXML(name, utf);

	// populate RSM
	std::vector<std::string> saved;

	std::map<std::string, RSM*>::const_iterator rsm_itr;

	bool found = false;
	std::string fname;
	for (unsigned int i = 0; i < getObjectCount(); i++) {
		const RSW::ModelObject* mdl = getModelObject(i);
		if (mdl == NULL)
			continue;

		fname = mdl->modelName;
		found = false;
		for(std::vector<std::string>::const_iterator itr = saved.begin(); itr != saved.end(); itr++) {
			if (*itr == fname)
				found = true;
		}

		if (!found) {
			saved.push_back(fname);
			rsm_itr = rsm.find(fname);
			if (rsm_itr != rsm.end()) {
				TiXmlElement* e = rsm_itr->second->GenerateXML(fname, utf);
				root->LinkEndChild(e);
			}
			else {
				std::cout << "can't find file " << fname << "!" << std::endl;
			}
		}
	}

	return(root);
}

TiXmlDocument RSW::GenerateFullXMLDoc(const std::map<std::string, RSM*> rsm, const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateFullXML(rsm, name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

TiXmlDocument RSW::GenerateFullXMLDoc(const std::map<std::string, RSM> rsm, const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateFullXML(rsm, name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

bool RSW::SaveFullXML(const std::map<std::string, RSM*> rsm, std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	out << doc;
	return(true);
}

bool RSW::SaveFullXML(const std::map<std::string, RSM> rsm, std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	out << doc;
	return(true);
}

bool RSW::SaveFullXML(const std::map<std::string, RSM*> rsm, const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	doc.SaveFile(fn);
	return(true);
}

bool RSW::SaveFullXML(const std::map<std::string, RSM> rsm, const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	doc.SaveFile(fn);
	return(true);
}

#endif

} /* namespace ro */
