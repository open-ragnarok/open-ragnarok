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

#include "ro/types/gat.h"
#include "ro/ro.h"

RO::GAT::GAT() {
}

RO::GAT::~GAT() {
}

bool RO::GAT::readStream(std::istream& s) {
	readHeader(s);

	if (!checkHeader(GAT_HEADER)) {
		std::cerr << "Invalid header for GAT file " << magic[0] << magic[1] << magic[2] << magic[3] << std::endl;
		return(false);
	}

	s.read((char*)&m_width, sizeof(unsigned int));
	s.read((char*)&m_height, sizeof(unsigned int));

	m_blocks = new strBlock[m_width * m_height];
	s.read((char*)m_blocks, sizeof(strBlock) * m_width * m_height);

	return(true);
}

bool RO::GAT::writeStream(std::ostream& o) const {
	writeHeader(o);

	o.write((char*)&m_width, sizeof(unsigned int));
	o.write((char*)&m_height, sizeof(unsigned int));
	o.write((char*)m_blocks, sizeof(strBlock) * m_width * m_height);
	return(true);
}

unsigned int RO::GAT::getWidth() const { return(m_width); }
unsigned int RO::GAT::getHeight() const { return(m_height); }

const RO::GAT::strBlock& RO::GAT::getBlock(const unsigned int& idx) const { return(m_blocks[idx]); }
RO::GAT::strBlock& RO::GAT::getBlock(const unsigned int& idx) { return(m_blocks[idx]); }

const RO::GAT::strBlock& RO::GAT::getBlock(const unsigned int& x, const unsigned int& y) const {
	 return(m_blocks[x + y * m_width]);
}

RO::GAT::strBlock& RO::GAT::getBlock(const unsigned int& x, const unsigned int& y) {
	 return(m_blocks[x + y * m_width]);
}

const RO::GAT::strBlock& RO::GAT::operator[] (const unsigned int& idx) const { return(m_blocks[idx]); }
RO::GAT::strBlock& RO::GAT::operator[] (const unsigned int& idx) { return(m_blocks[idx]); }

