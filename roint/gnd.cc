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

#include "ro/types/gnd.h"
#include "ro/ro.h"

// TODO use proper default values
static RO::GND::Lightmap g_emptyLightmap = {
	{
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 }
	},{
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} },
		{ {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} }
	}
};
static RO::GND::Surface g_emptySurface = {
	0,0,0,0,
	0,0,0,0,
	-1,
	0xFFFF,
	0,0,0,0
};
static RO::GND::Cell g_emptyCell = {
	0,0,0,0,
	-1,
	-1,
	-1
};

RO::GND::GND() {
	m_width = 0;
	m_height = 0;
	m_zoom = 10.0f;
	m_nTextures = 0;
	m_textureSize = 0;
	m_textures = NULL;
}

RO::GND::~GND() {
	reset();
}

void RO::GND::reset() {
	m_valid = false;
	m_width = 0;
	m_height = 0;
	m_zoom = 10.0f;
	m_nTextures = 0;
	m_textureSize = 0;
	if (m_textures != NULL) {
		delete[] m_textures;
		m_textures = NULL;
	}
	m_lightmaps.clear();
	m_surfaces.clear();
	m_cells.clear();
}

bool RO::GND::readStream(std::istream& s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(GND_HEADER)) {
		_log(ROINT__DEBUG, "Invalid GND header (%c%c%c%c)", magic[0], magic[1], magic[2], magic[3]);
		return(false);
	}

	if (m_version.cver.major == 1 && m_version.cver.minor == 7) {
		// supported [1.7]
	}
	else {
		_log(ROINT__DEBUG, "Unsupported GND version (%u.%u)", m_version.cver.major, m_version.cver.minor);
		return(false);
	}

	int i;
	int width, height;
	int nTextures, textureSize;

	s.read((char*)&width, sizeof(int));
	s.read((char*)&height, sizeof(int));
	s.read((char*)&m_zoom, sizeof(float));
	s.read((char*)&nTextures, sizeof(int));
	s.read((char*)&textureSize, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	// size
	if (width < 0 || height < 0) {
		_log(ROINT__DEBUG, "Invalid GND size (width=%d height=%d)", width, height);
		reset();
		return(false);
	}
	m_width = (unsigned int)width;
	m_height = (unsigned int)height;

	// textures
	if (nTextures < 0 || textureSize < 0) {
		_log(ROINT__DEBUG, "Invalid GND texture size (nTextures=%d textureSize=%d)", nTextures, textureSize);
		reset();
		return(false);
	}
	m_nTextures = (unsigned int)nTextures;
	m_textureSize = (unsigned int)textureSize;
	m_textures = new char[m_nTextures * m_textureSize];
	s.read(m_textures, m_nTextures * m_textureSize);
	for (i = 0; i < nTextures; i++) {
		char* texture = &m_textures[i * m_textureSize];
		texture[m_textureSize - 1] = '\0';
	}

	// read lightmaps
	int nLightmaps, lmapWidth, lmapHeight, lmapCells;
	s.read((char*)&nLightmaps, sizeof(int));
	s.read((char*)&lmapWidth, sizeof(int));
	s.read((char*)&lmapHeight, sizeof(int));
	s.read((char*)&lmapCells, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	if (nLightmaps < 0 || lmapWidth != 8 || lmapHeight != 8 || lmapCells != 1) {
		_log(ROINT__DEBUG, "Invalid GND lightmap size (nLightmaps=%d lmapWidth=%d lmapHeight=%d lmapCells=%d)", nLightmaps, lmapWidth, lmapHeight, lmapCells);
		reset();
		return(false);
	}
	m_lightmaps.resize((unsigned int)nLightmaps);
	for (i = 0; i < nLightmaps; i++) {
		Lightmap& lightmap = m_lightmaps[i];
		s.read((char*)&lightmap, sizeof(Lightmap));
	}

	// read surfaces
	int nSurfaces;
	s.read((char*)&nSurfaces, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	if (nSurfaces < 0) {
		_log(ROINT__DEBUG, "Invalid GND surface size (nSurfaces=%d)", nSurfaces);
		reset();
		return(false);
	}
	m_surfaces.resize((unsigned int)nSurfaces);
	for (i = 0; i < nSurfaces; i++) {
		Surface& surface = m_surfaces[i];
		s.read((char*)&surface, sizeof(Surface));
	}

	// read cells
	int nCells = m_width * m_height;
	m_cells.resize((unsigned int)nCells);
	for (i = 0; i < nCells; i++) {
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

bool RO::GND::writeStream(std::ostream& s) const {
	if (!isValid() || !writeHeader(s)) {
		return(false);
	}
	int width = (int)m_width;
	int height = (int)m_height;
	int nTextures = (int)m_nTextures;
	int textureSize = (int)m_textureSize;

	// write size
	s.write((char*)&width, sizeof(int));
	s.write((char*)&height, sizeof(int));
	s.write((char*)&m_zoom, sizeof(float));

	// write textures
	s.write((char*)&nTextures, sizeof(int));
	s.write((char*)&textureSize, sizeof(int));
	s.write(m_textures, m_nTextures * m_textureSize);

	// write lightmaps
	int nLightmaps = (int)m_lightmaps.size();
	int lmapWidth = 8;
	int lmapHeight = 8;
	int lmapCells = 1;
	s.write((char*)&nLightmaps, sizeof(int));
	s.write((char*)&lmapWidth, sizeof(int));
	s.write((char*)&lmapHeight, sizeof(int));
	s.write((char*)&lmapCells, sizeof(int));
	for (unsigned int i = 0; i < m_lightmaps.size(); i++) {
		const Lightmap& lightmap = m_lightmaps[i];
		s.write((char*)&lightmap, sizeof(Lightmap));
	}

	// write surfaces
	int nSurfaces = (int)m_surfaces.size();
	for (unsigned int i = 0; i < m_surfaces.size(); i++) {
		const Surface& surface = m_surfaces[i];
		s.write((char*)&surface, sizeof(Surface));
	}

	// write cells
	for (unsigned i = 0; i < m_cells.size(); i++) {
		const Cell& cell = m_cells[i];
		s.write((char*)&cell, sizeof(Cell));
	}

	return(!s.fail());
}

unsigned int RO::GND::getWidth() const {
	return(m_width);
}

unsigned int RO::GND::getHeight() const {
	return(m_height);
}

float RO::GND::getZoom() const {
	return(m_zoom);
}

unsigned int RO::GND::getTextureCount() const {
	return(m_nTextures);
}

const char* RO::GND::getTexture(unsigned int idx) const {
	if (idx < m_nTextures)
		return(&m_textures[idx * m_textureSize]);
	return("");
}

unsigned int RO::GND::getSurfaceCount() const {
	return(m_surfaces.size());
}

const RO::GND::Surface& RO::GND::getSurface(unsigned int idx) const {
	if (idx < m_surfaces.size())
		return(m_surfaces[idx]);
	return(g_emptySurface);
}

unsigned int RO::GND::getLightmapCount() const {
	return(m_lightmaps.size());
}

const RO::GND::Lightmap& RO::GND::getLightmap(unsigned int idx) const {
	if (idx < m_lightmaps.size())
		return(m_lightmaps[idx]);
	return(g_emptyLightmap);
}

unsigned int RO::GND::getCellCount() const {
	return(m_cells.size());
}

const RO::GND::Cell& RO::GND::operator [] (unsigned int idx) const {
	if (idx < m_cells.size())
		return(m_cells[idx]);
	return(g_emptyCell);
}

const RO::GND::Cell& RO::GND::getCell(unsigned int idx) const {
	if (idx < m_cells.size())
		return(m_cells[idx]);
	return(g_emptyCell);
}

const RO::GND::Cell& RO::GND::getCell(unsigned int cellx, unsigned int celly) const {
	if (cellx < m_width && celly < m_height)
		return(m_cells[cellx + celly * m_width]);
	return(g_emptyCell);
}

void RO::GND::Dump(std::ostream& o) const {
	char buf[512];
	unsigned int i;
	o << "Magic: " << magic[0] << magic[1] << magic[2] << magic[3] << std::endl;
	o << "Version: " << (int)m_version.cver.major << "." << (int)m_version.cver.minor << std::endl;
	sprintf(buf, "%f", m_zoom);
	o << "Size: width=" << m_width << " height=" << m_height << " zoom=" << buf << std::endl;

	o << "Textures: (" << getTextureCount() << ")" << std::endl;
	for (i = 0; i < getTextureCount(); i++) {
		const char* texture = getTexture(i);
		o << "\t" << texture << std::endl;
	}

	o << "Lightmaps: (" << m_lightmaps.size() << ")" << std::endl;
	for (i = 0; i < m_lightmaps.size(); i++) {
		const Lightmap& lightmap = m_lightmaps[i];
		unsigned int k, m;

		o << "\t{" << std::endl;
		for (k = 0; k < 8; k++) {
			o << "\t\t{ ";
			for (m = 0; m < 8; m++) {
				o << lightmap.brightness[k][m];
				if (m != 8) o << ",";
			}
			o << " }";
			if (k != 8) o << ",";
			o << std::endl;
		}
		o << "\t},{" << std::endl;
		for (k = 0; k < 8; k++) {
			o << "\t\t{ ";
			for (m = 0; m < 8; m++) {
				o << "{" << (int)lightmap.color[k][m][0] << "," << (int)lightmap.color[k][m][1] << "," << (int)lightmap.color[k][m][2];
				if (m != 8) o << ",";
			}
			o << " }";
			if (k != 8) o << ",";
			o << std::endl;
		}
		o << "\t}" << std::endl;
	}

	o << "Surfaces: " << m_surfaces.size() << std::endl;
	for (i = 0; i < m_surfaces.size(); i++) {
		unsigned int k;
		const Surface& surface = m_surfaces[i];
		for (k = 0; k < 4; k++) {
			sprintf(buf, "Texture coord: u=%f v=%f", surface.u[i], surface.v[i]);
			o << "\t" << buf << std::endl;
		}
		o << "\tTexture id: " << surface.textureId;
		o << "\tLightmap id: " << surface.lightmapId;
		o << "\tColor: b=" << (int)surface.color.b << " g=" << (int)surface.color.g << " r=" << (int)surface.color.r << " a=" << (int)surface.color.a << std::endl;
	}

	o << "Cells: " << m_cells.size() << std::endl;
	for (i = 0; i < m_cells.size(); i++) {
		const Cell& cell = m_cells[i];
		o << "[" << i << "] x=" << (i%8) << " y=" << (i/8) << std::endl;
		sprintf(buf, "Height: {%f %f %f %f}", cell.height[0], cell.height[1], cell.height[2], cell.height[3]);
		o << "\t" << buf << std::endl;
		o << "\tTop surface id: " << cell.topSurfaceId;
		o << "\tFront surface id: " << cell.frontSurfaceId;
		o << "\tRight surface id: " << cell.rightSurfaceId;
	}
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::GND::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement* root = new TiXmlElement("GND");
	char buf[512];
	unsigned int i;
	sprintf(buf,"%d.%d", m_version.cver.major , m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	// size
	sprintf(buf, "%u", m_width);
	root->SetAttribute("width", buf);
	sprintf(buf, "%u", m_height);
	root->SetAttribute("height", buf);
	sprintf(buf, "%f", m_zoom);
	root->SetAttribute("zoom", buf);

	// textures
	TiXmlElement* texturesxml = new TiXmlElement("textures");
	root->LinkEndChild(texturesxml);
	for (i = 0; i < m_nTextures; i++) {
		const char* texture = getTexture(i);
		TiXmlElement* texturexml = new TiXmlElement("texture");
		texturesxml->LinkEndChild(texturexml);
		if (utf)
			texturexml->SetValue(euc2utf8(texture));
		else
			texturexml->SetValue(texture);
	}

	// lightmaps
	TiXmlElement* lightmapsxml = new TiXmlElement("lightmaps");
	root->LinkEndChild(lightmapsxml);
	for (i = 0; i < m_lightmaps.size(); i++) {
		unsigned int k, m;
		const Lightmap& lightmap = m_lightmaps[i];
		TiXmlElement* lightmapxml = new TiXmlElement("lightmap");
		lightmapsxml->LinkEndChild(lightmapxml);
		for (k = 0; k < 8; k++) {
			for (m = 0; m < 8; m++) {
				TiXmlElement* pixelxml = new TiXmlElement("pixel");
				lightmapxml->LinkEndChild(pixelxml);
				pixelxml->SetAttribute("brightness", (int)lightmap.brightness[k][m]);
				pixelxml->SetAttribute("red", (int)lightmap.color[k][m].r);
				pixelxml->SetAttribute("green", (int)lightmap.color[k][m].g);
				pixelxml->SetAttribute("blue", (int)lightmap.color[k][m].b);
			}
		}
	}

	// surfaces
	TiXmlElement* surfacesxml = new TiXmlElement("surfaces");
	root->LinkEndChild(surfacesxml);
	for (i = 0; i < m_surfaces.size(); i++) {
		unsigned int k;
		const Surface& surface = m_surfaces[i];
		TiXmlElement* surfacexml = new TiXmlElement("surface");
		surfacesxml->LinkEndChild(surfacexml);
		for (k = 0; k < 4; k++) {
			TiXmlElement* verticexml = new TiXmlElement("tvertex");
			surfacexml->LinkEndChild(verticexml);
			sprintf(buf, "%f", surface.u[k]);
			surfacexml->SetAttribute("u", buf);
			sprintf(buf, "%f", surface.v[k]);
			surfacexml->SetAttribute("v", buf);
		}
		surfacexml->SetAttribute("textureid", (int)surface.textureId);
		surfacexml->SetAttribute("lightmapid", (int)surface.lightmapId);
	}

	// cells
	TiXmlElement* cellsxml = new TiXmlElement("cells");
	root->LinkEndChild(cellsxml);
	for (i = 0; i < m_cells.size(); i++) {
		unsigned int k;
		const Cell& cell = m_cells[i];
		TiXmlElement* cellxml = new TiXmlElement("cell");
		cellsxml->LinkEndChild(cellxml);

		for (k = 0; k < 4; k++) {
			TiXmlElement* heightxml = new TiXmlElement("height");
			cellxml->LinkEndChild(heightxml);
			sprintf(buf, "%f", cell.height[k]);
			heightxml->SetValue(buf);
		}
		cellxml->SetAttribute("topsurfaceid", cell.topSurfaceId);
		cellxml->SetAttribute("frontsurfaceid", cell.frontSurfaceId);
		cellxml->SetAttribute("rightsurfaceid", cell.rightSurfaceId);
	}
	return(root);
}
#endif
