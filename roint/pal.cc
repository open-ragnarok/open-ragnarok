/* $Id$ */
#include "stdafx.h"
#include "ro/types/pal.h"

RO::PAL::PAL() : Object() {
	memset(m_pal, 0, sizeof(m_pal));
}

RO::PAL::~PAL() {
}

bool RO::PAL::readStream(std::istream& s) {
	s.read((char*)&m_pal, sizeof(m_pal));
	return(!s.fail());
}

const RO::PAL::Pal* RO::PAL::getPal(const unsigned char idx) const {
	return(&m_pal[idx]);
}

