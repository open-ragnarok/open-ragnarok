/* $Id: str.cc -1 $ */
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
#include "ro/types/str.h"

namespace ro {

STR::STR() : Object() {
//	memset(m_pal, 0, sizeof(m_pal));
}

STR::~STR() {
	reset();
}
void STR::reset() {
	m_valid = false;
//	memset(m_pal, 0, sizeof(m_pal));
}

bool STR::readStream(std::istream& s) {
//	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(STR_HEADER)) {
		_log(ROINT__DEBUG, "Invalid STR header (%c%c%c%c)", magic[0], magic[1], magic[2], magic[3]);
		return(false);
	}

/*	if (m_version.cver.major == 1 && m_version.cver.minor >= 1 && m_version.cver.minor <= 5) {
		// supported [1.1 1.5]
	}
	else {
		_log(ROINT__DEBUG, "Unsupported STR version (%u.%u)", m_version.cver.major, m_version.cver.minor);
		return(false);
	}*/

	s.ignore(2);

	int nlayer, n;
	s.read((char*)&m_fps, sizeof(int));
	s.read((char*)&m_maxframe, sizeof(int));

	// read layers
	s.read((char*)&nlayer, sizeof(int));
	nlayer--;
	s.read(m_reserved, sizeof(m_reserved));
	if (s.fail()) {
	//	reset();
		return(false);
	}
	if (nlayer > 0) {
		m_layers.resize((unsigned int)nlayer);
		for (int i = 0; i < nlayer; i++) {
			Layer& layer = m_layers[i];
			// read texture names
			s.read((char*)&n, sizeof(int));
			if (s.fail()) {
			//	reset();
				return(false);
			}
			layer.textures.resize(n);
			for (int j = 0; j < n; j++) {
//				s.read((char*)&layer.textures[j].name[0], sizeof(Texture));
				s.read(layer.textures[j].name, sizeof(layer.textures[j].name));
//				s.read(layer.textures[j].name, 128);
			}
			s.read((char*)&n, sizeof(int));
			if (s.fail()) {
			//	reset();
				return(false);
			}
			layer.frames.resize(n);
			for (int j = 0; j < n; j++) {
				s.read((char*)&layer.frames[j], sizeof(Frame));
			}
		}
	}

	if (s.fail()) {
	//	reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

const STR::Layer& STR::getLayer(int index) const {
	return m_layers[index];
}

int STR::getLayerCount() const {
	return m_layers.size();
}

int STR::getMaxFrame() const {
	return m_maxframe;
}

} /* namespace ro */
