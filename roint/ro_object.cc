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
#include "ro/ro_object.h"

#include <fstream>

RO::Object::Object() {
	m_valid = false;
	m_version.sver = 0;
	magicSize = 4;
	memset(magic, 0, 4);
}

RO::Object::Object(const Object& o) {
	memcpy(magic, o.magic, 4);
	magicSize = o.magicSize;
	m_version.sver = o.m_version.sver;
	m_valid = o.m_valid;
}

RO::Object::~Object() {
}

const RO::s_obj_ver* RO::Object::getVersion() const {
	return(&m_version);
}

bool RO::Object::IsCompatibleWith(unsigned char major, unsigned char minor) const {
	return (
		((m_version.cver.major == major) && (m_version.cver.minor >= minor))
		|| (m_version.cver.major > major)
	);
}

bool RO::Object::IsCompatibleWith(short ver) const {
	s_obj_ver v;
	v.sver = ver;
	return(IsCompatibleWith(v.cver.major, v.cver.minor));
}

bool RO::Object::readHeader(std::istream &s) {
	s.read((char*)&magic, magicSize);
	s.read((char*)&m_version, 2);
	return(!s.fail());
}

bool RO::Object::writeHeader(std::ostream &s) const {
	s.write((char*)&magic, magicSize);
	s.write((char*)&m_version, 2);
	return(!s.fail());
}

bool RO::Object::copyHeader(Object* o) const {
	memcpy(o->magic, magic, 4);
	o->magicSize = magicSize;
	o->m_version.sver = m_version.sver;
	o->m_valid = m_valid;
	return(true);
}

bool RO::Object::copyHeader(Object& o) const {
	return(copyHeader(&o));
}

bool RO::Object::isValid() const {
	return(m_valid);
}

bool RO::Object::checkHeader(const std::string& s) const {
	if (strncmp(s.c_str(), magic, magicSize))
		return(false);
	return(true);
}


bool RO::Object::read(const std::string& fn) {
	std::ifstream fp(fn.c_str(), std::ios_base::binary);
	bool ret = readStream(fp);
	fp.close();
	return(ret);
}

#ifdef ROINT_USE_XML
TiXmlElement *RO::Object::GenerateXML(const std::string& name, bool utf) const {
	char buf[32];
	memset(buf, 0, 32);
	strncpy(buf, magic, magicSize);

	TiXmlElement *root = new TiXmlElement(buf);
	sprintf(buf,"%d.%d", m_version.cver.major, m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	return(root);
}

TiXmlDocument RO::Object::GenerateXMLDoc(const std::string& name, bool utf) const {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement * root = GenerateXML(name, utf);
	doc.LinkEndChild(root);
	
	return(doc);
}

bool RO::Object::SaveXML(std::ostream& out, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	out << doc;
	return(true);
}

bool RO::Object::SaveXML(const std::string& fn, const std::string& name, bool utf) const {
	TiXmlDocument doc = GenerateXMLDoc(name, utf);
	doc.SaveFile(fn);
	return(true);
}

#endif
