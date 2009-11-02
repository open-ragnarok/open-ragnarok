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

static RO::GAT::Cell g_emptyCell = {0,0,0,0,1};

RO::GAT::GAT() {
}

RO::GAT::~GAT() {
	reset();
}

void RO::GAT::reset(void) {
	m_valid = false;
	m_width = m_height = 0;
	m_cells.clear();
}

bool RO::GAT::readStream(std::istream& s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(GAT_HEADER)) {
		_log(ROINT__DEBUG, "Invalid GAT header (%c%c%c%c)", magic[0], magic[1], magic[2], magic[3]);
		return(false);
	}

	if (m_version.cver.major == 1 && m_version.cver.minor == 2) {
		// supported [1.2]
	}
	else {
		_log(ROINT__DEBUG, "Unsupported GAT version (%u.%u)", m_version.cver.major, m_version.cver.minor);
		return(false);
	}

	s.read((char*)&m_width, sizeof(unsigned int));
	s.read((char*)&m_height, sizeof(unsigned int));

	m_cells.resize(m_width * m_height);
	for (unsigned int i = 0; i < m_cells.size(); i++) {
		Cell& cell = m_cells[i];
		s.read((char*)&cell, sizeof(Cell));
	}

	if (s.fail()) {
		reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

bool RO::GAT::writeStream(std::ostream& s) const {
	if (!isValid() || !writeHeader(s))
		return(false);

	s.write((char*)&m_width, sizeof(unsigned int));
	s.write((char*)&m_height, sizeof(unsigned int));
	for (unsigned int i = 0; i < m_cells.size(); i++) {
		const Cell& cell = m_cells[i];
		s.write((char*)&cell, sizeof(Cell));
	}
	return(!s.fail());
}

unsigned int RO::GAT::getWidth() const {
	return(m_width);
}

unsigned int RO::GAT::getHeight() const {
	return(m_height);
}

unsigned int RO::GAT::getCellCount(void) const {
	return(m_cells.size());
}

const RO::GAT::Cell& RO::GAT::operator[] (unsigned int idx) const {
	return(m_cells[idx]);
}

const RO::GAT::Cell& RO::GAT::getCell(unsigned int idx) const {
	if (idx < m_cells.size())
		return(m_cells[idx]);
	return(g_emptyCell);
}

const RO::GAT::Cell& RO::GAT::getCell(unsigned int cellx, unsigned int celly) const {
	if (cellx < m_width && celly < m_height)
		return(m_cells[cellx + celly * m_width]);
	return(g_emptyCell);
}

float RO::GAT::getAltitude(float cellx, float celly) const {
	if (cellx >= 0.0f && cellx < m_width && celly >= 0.0f && celly < m_height) {
		unsigned int x = (unsigned int)cellx;
		unsigned int y = (unsigned int)celly;
		const Cell& cell = m_cells[x + y * m_width];
		float offy = celly - y;
		float west = (cell.height[2] - cell.height[0]) * offy + cell.height[0];
		float east = (cell.height[3] - cell.height[1]) * offy + cell.height[1];
		float offx = cellx - x;
		return((east - west) * offx + west);
	}
	return(0.0f);
}

float RO::GAT::getAltitude(unsigned int cellx, unsigned int celly) const {
	if (cellx < m_width && celly < m_height) {
		const Cell& cell = m_cells[cellx + celly * m_width];
		return((cell.height[0] + cell.height[1] + cell.height[2] + cell.height[3]) * 0.25f);
	}
	return(0.0f);
}

int RO::GAT::getType(unsigned int cellx, unsigned int celly) const {
	if (cellx < m_width && celly < m_height)
		return(m_cells[cellx + celly * m_width].type);
	return(1);// non-walkable
}
