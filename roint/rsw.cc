/* $Id$ */
#include "stdafx.h"
#include "rsw.h"
#include "ro.h"

/*
RO::RSW::OT_Unknown = 0;
RO::RSW::OT_Model = 1;
RO::RSW::OT_Light = 2;
RO::RSW::OT_Sound = 3;
RO::RSW::OT_Effect = 4;
*/

RO::RSW::RSW() : RO::Object() {
	object_count = 0;
	m_objects = NULL;
}

RO::RSW::RSW(const RSW& rsw) : RO::Object(rsw) {
	object_count = rsw.object_count;
	m_objects = new Object*[object_count];
	for(unsigned int i = 0; i < object_count; i++) {
		m_objects[i] = rsw.m_objects[i]->Copy();
	}
	strcpy(ini_file, rsw.ini_file);
	strcpy(gnd_file, rsw.gnd_file);
	strcpy(gat_file, rsw.gat_file);
	strcpy(scr_file, rsw.scr_file);

	memcpy(&water, &rsw.water, sizeof(strWater));
	memcpy(&light, &rsw.light, sizeof(strLight));

	memcpy(unk, rsw.unk, 3);
}

//RO::RSW::RSW(const RSW* rsw) {
//	*this = *rsw;
//}

RO::RSW::~RSW() {
	Clear();
}

bool RO::RSW::readStream(std::istream& s) {
	readHeader(s);

	if (!checkHeader(RSW_HEADER)) {
		std::cout << "Invalid RSW header (" << magic[0] << magic[1] << magic[2] << magic[3] << ")" << std::endl;
		return(false);
	}

	s.read(ini_file, 40);
	s.read(gnd_file, 40);

	if (IsCompatibleWith(1, 4))
		s.read(gat_file, 40);
	else
		memset(gat_file, 0, 40);
	s.read(scr_file, 40);

	// Water
	if (IsCompatibleWith(1,3))
		s.read((char*)&water.height, sizeof(float));
	else
		water.height = 0.0f;

	if( IsCompatibleWith(1,8) ) {
		s.read((char*)&water.type, sizeof(int) + sizeof(float) * 3);
		//water.type = readInt(s);
		//water.amplitude = readFloat(s);
		//water.phase = readFloat(s);
		//water.surface_curve_level = readFloat(s);
	}
	else {
		water.type = 0;
		water.amplitude = 1.0;
		water.phase = 2.0;
		water.surface_curve_level = 0.5;
	}

	if( IsCompatibleWith(1,9) )
		s.read((char*)&water.texture_cycling, sizeof(int));
	else
		water.texture_cycling = 3;

	if( IsCompatibleWith(1,4) ) {
		s.read((char*)&light, sizeof(strLight));
	}
	else {
		memset((char*)&light, 0, sizeof(strLight));
	}

	s.read((char*)unk, sizeof(int) * 3);

	s.read((char*)&object_count, sizeof(unsigned int));
	m_objects = new Object*[object_count];

	for (unsigned int i = 0; i < object_count; i++) {
		m_objects[i] = readObject(s);
		if (m_objects[i] == NULL)
			return(false);
	}

	return(true);
}

RO::RSW::Object* RO::RSW::readObject(std::istream& in) {
	int objtype;
	Object* o;

	in.read((char*)&objtype, sizeof(int));
	switch (objtype) {
		case 1:
			o = new Model();
			break;
		case 2:
			o = new Light();
			break;
		case 3:
			o = new Sound();
			break;
		case 4:
			o = new Effect();
			break;
		default:
			fprintf(stderr, "ERROR! invalid object %u\n", objtype);
			return(NULL);
	}
	o->readStream(in);

	return(o);
}

bool RO::RSW::writeStream(std::ostream&) const {
	return(true);
}

void RO::RSW::Dump(std::ostream& o, const std::string& pfx) const {
	char buf[1024];
	sprintf(buf,"Magic: %c%c%c%c", magic[0], magic[1], magic[2], magic[3]);
	o << pfx << buf << std::endl;
	sprintf(buf,"Version: %u.%u", m_version.cver.major, m_version.cver.minor);
	o << pfx << buf << std::endl;

	o << pfx << "Ini: " << ini_file << std::endl;
	o << pfx << "Gnd: " << gnd_file << std::endl;
	o << pfx << "Gat: " << gat_file << std::endl;
	o << pfx << "Scr: " << scr_file << std::endl;

	sprintf(buf,"Water");
	o << pfx << buf << std::endl;
	sprintf(buf,"\tHeight: %f", water.height);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tType: %u", water.type);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tAmpliture: %.2f", water.amplitude);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tPhase: %.2f", water.phase);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tSurface curve level: %.2f", water.surface_curve_level);
	o << pfx << buf << std::endl;

	sprintf(buf,"Light");
	o << pfx << buf << std::endl;
	sprintf(buf,"\tAmbient: %.2f %.2f %.2f", light.ambient[0], light.ambient[1], light.ambient[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tDiffuse: %.2f %.2f %.2f", light.diffuse[0], light.diffuse[1], light.diffuse[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tShadow: %.2f %.2f %.2f", light.shadow[0], light.shadow[1], light.shadow[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"\tAlpha: %.2f", light.alpha);
	o << pfx << buf << std::endl;
	sprintf(buf,"Unknown: %d (0x%x)", unk[0], unk[0]);
	o << pfx << buf << std::endl;
	sprintf(buf,"Unknown: %d (0x%x)", unk[1], unk[1]);
	o << pfx << buf << std::endl;
	sprintf(buf,"Unknown: %d (0x%x)", unk[2], unk[2]);
	o << pfx << buf << std::endl;
	sprintf(buf,"Objects: %u", object_count);
	o << pfx << buf << std::endl;

	unsigned int i;
	for (i = 0; i < object_count; i++) {
		m_objects[i]->Dump(o);
	}
}

void RO::RSW::Dump(std::ostream& o) const {
	Dump(o, "");
}

void RO::RSW::Clear() {
	if (object_count > 0) {
		for (unsigned int i = 0; i < object_count; i++) {
			delete m_objects[i];
		}
		delete[] m_objects;
	}
	m_objects = NULL;
	object_count = 0;
}

unsigned int RO::RSW::getObjectCount() const {
	return(object_count);
}

RO::RSW::Object* RO::RSW::getObject(const unsigned int& idx) {
	return(m_objects[idx]);
}

const RO::RSW::Object* RO::RSW::getObject(const unsigned int& idx) const {
	return(m_objects[idx]);
}

RO::RSW::Object* RO::RSW::operator[] (const unsigned int& idx) {
	return(m_objects[idx]);
}

const RO::RSW::Object* RO::RSW::operator[] (const unsigned int& idx) const {
	return(m_objects[idx]);
}

RO::RSW& RO::RSW::operator = (const RO::RSW& rsw) {
	Clear();
	rsw.copyHeader(this);
	object_count = rsw.object_count;
	m_objects = new Object*[object_count];
	for(unsigned int i = 0; i < object_count; i++) {
		m_objects[i] = rsw.m_objects[i]->Copy();
	}
	strcpy(ini_file, rsw.ini_file);
	strcpy(gnd_file, rsw.gnd_file);
	strcpy(gat_file, rsw.gat_file);
	strcpy(scr_file, rsw.scr_file);

	memcpy(&water, &rsw.water, sizeof(strWater));
	memcpy(&light, &rsw.light, sizeof(strLight));

	memcpy(unk, rsw.unk, 3);

	return(*this);
}

/* === XML === */
#ifdef ROINT_USE_XML

#include <fstream>

TiXmlElement *RO::RSW::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("RSW");

	char buf[16];
	sprintf(buf,"%d.%d", m_version.cver.major , m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") root->SetAttribute("name", name);
	// if (utf) std::cout << "Saving XML with UTF-8 Encoding" << std::endl;

	if (strlen(ini_file)) {
		TiXmlElement *file = new TiXmlElement("ini_file");
		std::string n;
		if (utf)
			n = euc2utf8(ini_file);
		else
			n = ini_file;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(gnd_file)) {
		TiXmlElement *file = new TiXmlElement("gnd_file");
		std::string n;
		if (utf)
			n = euc2utf8(gnd_file);
		else
			n = gnd_file;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(gat_file)) {
		TiXmlElement *file = new TiXmlElement("gat_file");
		std::string n;
		if (utf)
			n = euc2utf8(gat_file);
		else
			n = gat_file;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}
	if (strlen(scr_file)) {
		TiXmlElement *file = new TiXmlElement("scr_file");
		std::string n;
		if (utf)
			n = euc2utf8(scr_file);
		else
			n = scr_file;
		file->LinkEndChild(new TiXmlText(n));
		root->LinkEndChild(file);
	}

	// Water
	TiXmlElement *attr;
	TiXmlElement *water = new TiXmlElement("water");

	sprintf(buf, "%f", this->water.height);
	attr = new TiXmlElement("height");
	attr->LinkEndChild(new TiXmlText(buf));
	water->LinkEndChild(attr);

	sprintf(buf, "%u", this->water.type);
	attr = new TiXmlElement("type");
	attr->LinkEndChild(new TiXmlText(buf));
	water->LinkEndChild(attr);

	sprintf(buf, "%f", this->water.amplitude);
	attr = new TiXmlElement("amplitude");
	attr->LinkEndChild(new TiXmlText(buf));
	water->LinkEndChild(attr);

	sprintf(buf, "%f", this->water.phase);
	attr = new TiXmlElement("phase");
	attr->LinkEndChild(new TiXmlText(buf));
	water->LinkEndChild(attr);

	sprintf(buf, "%f", this->water.surface_curve_level);
	attr = new TiXmlElement("surface_curve_level");
	attr->LinkEndChild(new TiXmlText(buf));
	water->LinkEndChild(attr);

	root->LinkEndChild(water);

	attr = new TiXmlElement("light_ambient");
	sprintf(buf, "%f", this->light.ambient[0]);
	attr->SetAttribute("r", buf);
	sprintf(buf, "%f", this->light.ambient[1]);
	attr->SetAttribute("g", buf);
	sprintf(buf, "%f", this->light.ambient[2]);
	attr->SetAttribute("b", buf);
	root->LinkEndChild(attr);

	attr = new TiXmlElement("light_diffuse");
	sprintf(buf, "%f", this->light.diffuse[0]);
	attr->SetAttribute("r", buf);
	sprintf(buf, "%f", this->light.diffuse[1]);
	attr->SetAttribute("g", buf);
	sprintf(buf, "%f", this->light.diffuse[2]);
	attr->SetAttribute("b", buf);
	root->LinkEndChild(attr);

	attr = new TiXmlElement("light_shadow");
	sprintf(buf, "%f", this->light.shadow[0]);
	attr->SetAttribute("r", buf);
	sprintf(buf, "%f", this->light.shadow[1]);
	attr->SetAttribute("g", buf);
	sprintf(buf, "%f", this->light.shadow[2]);
	attr->SetAttribute("b", buf);
	root->LinkEndChild(attr);

	for (unsigned int i = 0; i < object_count; i++) {
		TiXmlElement *object = new TiXmlElement("object");
		switch(m_objects[i]->getType()) {
			case OT_Model:
				object->SetAttribute("type", "model");
				{
					Model* mdl = (Model*)m_objects[i];
					if (utf) {
						object->SetAttribute("filename", euc2utf8(mdl->data->filename));
						object->SetAttribute("name", euc2utf8(mdl->data->m_name));
					}
					else {
						object->SetAttribute("filename", mdl->data->filename);
						object->SetAttribute("name", mdl->data->m_name);
					}

					attr = new TiXmlElement("pos");
					sprintf(buf, "%f", mdl->data->pos[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->data->pos[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->data->pos[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);

					attr = new TiXmlElement("rot");
					sprintf(buf, "%f", mdl->data->rot[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->data->rot[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->data->rot[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);

					attr = new TiXmlElement("scale");
					sprintf(buf, "%f", mdl->data->scale[0]);
					attr->SetAttribute("x", buf);
					sprintf(buf, "%f", mdl->data->scale[1]);
					attr->SetAttribute("y", buf);
					sprintf(buf, "%f", mdl->data->scale[2]);
					attr->SetAttribute("z", buf);
					object->LinkEndChild(attr);
				}
				break;
			case OT_Effect:
				object->SetAttribute("type", "effect");
				break;
			case OT_Light:
				object->SetAttribute("type", "light");
				break;
			case OT_Sound:
				object->SetAttribute("type", "sound");
				break;
		}

		root->LinkEndChild(object);
	}

	return(root);
}

TiXmlElement *RO::RSW::GenerateFullXML(const std::map<std::string, RSM> rsm, const std::string& name, bool utf) const {
	TiXmlElement *root = GenerateXML(name, utf);

	// populate RSM
	std::vector<std::string> saved;
	std::map<std::string, RSM>::const_iterator rsm_itr;
	bool found = false;
	std::string fname;
	for (unsigned int i = 0; i < getObjectCount(); i++) {
		if (!getObject(i)->isType(RO::RSW::OT_Model))
			continue;

		RO::RSW::Model *mdl = (RO::RSW::Model*)getObject(i);
		fname = mdl->data->filename;
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


TiXmlElement *RO::RSW::GenerateFullXML(const std::map<std::string, RSM*> rsm, const std::string& name, bool utf) const {
	TiXmlElement *root = GenerateXML(name, utf);

	// populate RSM
	std::vector<std::string> saved;

	std::map<std::string, RSM*>::const_iterator rsm_itr;

	bool found = false;
	std::string fname;
	for (unsigned int i = 0; i < getObjectCount(); i++) {
		if (!getObject(i)->isType(RO::RSW::OT_Model))
			continue;

		RO::RSW::Model *mdl = (RO::RSW::Model*)getObject(i);
		fname = mdl->data->filename;
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

TiXmlDocument RO::RSW::GenerateXMLDoc(const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateXML(name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

TiXmlDocument RO::RSW::GenerateFullXMLDoc(const std::map<std::string, RSM*> rsm, const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateFullXML(rsm, name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

TiXmlDocument RO::RSW::GenerateFullXMLDoc(const std::map<std::string, RSM> rsm, const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateFullXML(rsm, name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

bool RO::RSW::SaveXML(std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	out << doc;
	return(true);
}

bool RO::RSW::SaveFullXML(const std::map<std::string, RSM*> rsm, std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	out << doc;
	return(true);
}

bool RO::RSW::SaveFullXML(const std::map<std::string, RSM> rsm, std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	out << doc;
	return(true);
}

bool RO::RSW::SaveXML(const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	doc.SaveFile(fn);
	return(true);
}

bool RO::RSW::SaveFullXML(const std::map<std::string, RSM*> rsm, const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	doc.SaveFile(fn);
	return(true);
}

bool RO::RSW::SaveFullXML(const std::map<std::string, RSM> rsm, const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateFullXMLDoc(rsm, name, utf);
	doc.SaveFile(fn);
	return(true);
}

#endif
