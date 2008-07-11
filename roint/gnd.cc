/* $id$ */
#include "stdafx.h"

#include "gnd.h"
#include "ro.h"

RO::GND::GND() {
	m_textures = NULL;
	m_cubes = NULL;
}

RO::GND::~GND() {
	if (m_textures != NULL)
		delete[] m_textures;
	m_textures = NULL;

	if (m_cubes != NULL)
		delete[] m_cubes;
	m_cubes = NULL;

}

bool RO::GND::readStream(std::istream& s) {
	readHeader(s);

	if (!checkHeader(GND_HEADER)) {
		std::cerr << "Invalid header for GND file " << magic[0] << magic[1] << magic[2] << magic[3] << std::endl;
		return(false);
	}

	s.read((char*)&m_gndheader, sizeof(strGndHeader));

	m_textures = new strTexture[m_gndheader.texture_count];
	s.read((char*)m_textures, sizeof(strTexture) * m_gndheader.texture_count);

	s.read((char*)&m_lightmapcount, sizeof(int));
	s.read((char*)&m_grid, sizeof(strGridInfo));
	m_lightmaps = new strLightmap[m_lightmapcount];
	s.read((char*)m_lightmaps, sizeof(strLightmap) * m_lightmapcount);

	m_tiles.readStream(s);

	m_cubecount = m_gndheader.size_x * m_gndheader.size_y;
	s.read((char*)&m_cubes, sizeof(strCube) * m_cubecount);

	return(true);
}

void RO::GND::Dump(std::ostream& o) const {
	o << "Magic: " << magic[0] << magic[1] << magic[2] << magic[3] << std::endl;
	o << "Version: " << (int)m_version.cver.major << "." << (int)m_version.cver.minor << std::endl;
	o << "Size: " << m_gndheader.size_x << " x " << m_gndheader.size_y << std::endl;
	o << "Textures (" << m_gndheader.texture_count << ")" << std::endl;
	for (unsigned i = 0; i < m_gndheader.texture_count; i++)
		o << "\t" << m_textures[i].path << std::endl;
}

const RO::GND::strTile& RO::GND::getTile(const unsigned int& idx) const {
	return(m_tiles[idx]);
}
RO::GND::strTile& RO::GND::getTile(const unsigned int& idx) {
	return(m_tiles[idx]);
}

const RO::GND::strTile& RO::GND::getTile(const unsigned int& x, const unsigned int& y) const {
	return(getTile(x + y * m_gndheader.size_x));
}

RO::GND::strTile& RO::GND::getTile(const unsigned int& x, const unsigned int& y) {
	return(getTile(x + y * m_gndheader.size_x));
}

const RO::GND::strCube& RO::GND::getCube(const unsigned int& x, const unsigned int& y) const {
	return(getCube(x + y & m_gndheader.size_x));
}

RO::GND::strCube& RO::GND::getCube(const unsigned int& x, const unsigned int& y) {
	return(getCube(x + y & m_gndheader.size_x));
}

const RO::GND::strCube& RO::GND::getCube(const unsigned int& idx) const {
	return(m_cubes[idx]);
}

RO::GND::strCube& RO::GND::getCube(const unsigned int& idx) {
	return(m_cubes[idx]);
}

const RO::GND::strTexture& RO::GND::getTexture(const unsigned int& idx) const {
	return(m_textures[idx]);
}

unsigned int RO::GND::getTextureCount() const {
	return(m_gndheader.texture_count);
}

unsigned int RO::GND::getTileCount() const {
	return(m_tiles.size());
}

unsigned int RO::GND::getCubeCount() const {
	return(m_cubecount);
}

RO::GND::strTexture& RO::GND::getTexture(const unsigned int& idx) {
	return(m_textures[idx]);
}

const char* RO::GND::getTextureName(const unsigned int& idx) const {
	return(m_textures[idx].path);
}

