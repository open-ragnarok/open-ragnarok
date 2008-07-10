/* $id$ */
#include "stdafx.h"
#include "object.h"

#include <fstream>

RO::Object::Object() {
	m_valid = false;
	m_version.sver = 0;
	magicSize = 4;
	memset(magic, 0, 4);
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
	// TODO: Check if we read all data.
	return(true);
}

bool RO::Object::writeHeader(std::ostream &s) const {
	s.write((char*)&magic, magicSize);
	s.write((char*)&m_version, 2);

	return(true);
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