/* $Id$ */
#include "stdafx.h"

#include "ro/types/gnd.h"
#include "ro/ro.h"

RO::GND::GND() {
	m_textures = NULL;
	m_cubes = NULL;
	m_lightmaps = NULL;
}

RO::GND::~GND() {
	Clear();
}

void RO::GND::Clear() {
	m_version.sver = 0;
	memset(magic, 0, 4);

	memset((char*)&m_gndheader, 0, sizeof(strGndHeader));
	memset((char*)&m_grid, 0, sizeof(strGridInfo));

	if (m_textures != NULL)
		delete[] m_textures;
	m_textures = NULL;

	if (m_cubes != NULL)
		delete[] m_cubes;
	m_cubes = NULL;

	if (m_lightmaps != NULL)
		delete[] m_lightmaps;
	m_lightmaps = NULL;

	m_tiles.Clear();
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
	m_cubes = new strCube[m_cubecount];
	s.read((char*)m_cubes, sizeof(strCube) * m_cubecount);

	return(true);
}

void RO::GND::Dump(std::ostream& o) const {
	o << "Magic: " << magic[0] << magic[1] << magic[2] << magic[3] << std::endl;
	o << "Version: " << (int)m_version.cver.major << "." << (int)m_version.cver.minor << std::endl;
	o << "Size: " << m_gndheader.size_x << " x " << m_gndheader.size_y << std::endl;
	o << "Tiles: " << m_tiles.getCount() << std::endl;
	o << "Cubes: " << m_cubecount << std::endl;
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

const RO::GND::strCube& RO::GND::getCube(const unsigned int& x, const unsigned int& y) const {
	return(getCube(x + y * m_gndheader.size_x));
}

RO::GND::strCube& RO::GND::getCube(const unsigned int& x, const unsigned int& y) {
	return(getCube(x + y * m_gndheader.size_x));
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

unsigned int RO::GND::getWidth() const {
	return(m_gndheader.size_x);
}

unsigned int RO::GND::getHeight() const {
	return(m_gndheader.size_y);
}

#ifdef ROINT_USE_XML
TiXmlElement* RO::GND::GenerateXML(const std::string& name, bool utf) const {
	TiXmlElement *root = new TiXmlElement("GND");
	char buf[16];
	unsigned int i;
	sprintf(buf,"%d.%d", m_version.cver.major , m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	// Size
	sprintf(buf, "%d", m_gndheader.size_x);
	root->SetAttribute("size_x", buf);
	sprintf(buf, "%d", m_gndheader.size_y);
	root->SetAttribute("size_y", buf);

	TiXmlElement* tiles = new TiXmlElement("tiles");
	root->LinkEndChild(tiles);
	for (i = 0; i < (unsigned int)m_tiles.getCount(); i++) {
		TiXmlElement* tile = new TiXmlElement("tile");
		tiles->LinkEndChild(tile);
		for (unsigned int j = 0; j < 4; j++) {
			TiXmlElement* texture = new TiXmlElement("texture");
			tile->LinkEndChild(texture);
			sprintf(buf, "%f", m_tiles[i].texture_start[j]);
			texture->SetAttribute("u", buf);
			sprintf(buf, "%f", m_tiles[i].texture_end[j]);
			texture->SetAttribute("v", buf);
		}
		sprintf(buf, "%d", m_tiles[i].texture_index);
		tile->SetAttribute("texture_index", buf);
		sprintf(buf, "%d", m_tiles[i].lightmap);
		tile->SetAttribute("lightmap", buf);
		TiXmlElement* color = new TiXmlElement("color");
		tile->LinkEndChild(color);
		
		// Red
		sprintf(buf, "%d", m_tiles[i].color[2]);
		color->SetAttribute("r", buf);

		// Green
		sprintf(buf, "%d", m_tiles[i].color[1]);
		color->SetAttribute("g", buf);
		
		// Blue
		sprintf(buf, "%d", m_tiles[i].color[0]);
		color->SetAttribute("b", buf);

		// Alpha
		sprintf(buf, "%d", m_tiles[i].color[3]);
		color->SetAttribute("a", buf);
	}

	// CUBES
	TiXmlElement* cubes = new TiXmlElement("cubes");
	root->LinkEndChild(cubes);
	for (i = 0; i < m_cubecount; i++) {
		TiXmlElement* cube = new TiXmlElement("cube");
		cubes->LinkEndChild(cube);
		sprintf(buf, "%f", m_cubes[i].height[0]);
		cube->SetAttribute("height_1", buf);
		sprintf(buf, "%f", m_cubes[i].height[1]);
		cube->SetAttribute("height_2", buf);
		sprintf(buf, "%f", m_cubes[i].height[2]);
		cube->SetAttribute("height_3", buf);
		sprintf(buf, "%f", m_cubes[i].height[3]);
		cube->SetAttribute("height_4", buf);

		sprintf(buf, "%d", m_cubes[i].tile_up);
		cube->SetAttribute("tile_up", buf);
		sprintf(buf, "%d", m_cubes[i].tile_side);
		cube->SetAttribute("tile_side", buf);
		sprintf(buf, "%d", m_cubes[i].tile_aside);
		cube->SetAttribute("tile_aside", buf);
	}

	// TEXTURES
	TiXmlElement* textures = new TiXmlElement("textures");
	root->LinkEndChild(textures);
	for (i = 0; i < m_gndheader.texture_count; i++) {
		TiXmlElement* texture = new TiXmlElement("texture");
		textures->LinkEndChild(texture);
		std::string n = m_textures[i].path;
		if (utf)
			n = euc2utf8(n);

		texture->SetAttribute("name", n);
		texture->SetAttribute("unk", m_textures[i].unk);
	}

	return(root);
}
#endif
