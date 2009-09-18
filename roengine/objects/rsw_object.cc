/* $Id$ */
#include "stdafx.h"

#include "roengine/objects/rsw_object.h"
#include "sdle/sdl_engine.h"

#include <GL/gl.h>
#include <GL/glu.h>

float RswObject::m_tilesize = 10.0f;

RswObject::RswObject(const RO::RSW* rsw, ROObjectCache& cache) : GLObject() {
	this->rsw = rsw;
	std::string gnd_fn = rsw->gnd_file;
	std::string gat_fn = rsw->gat_file;
	this->gnd = (RO::GND*)cache[gnd_fn];
	this->gat = (RO::GAT*)cache[gat_fn];
}

RswObject::~RswObject() {
}

const RO::RSW* RswObject::getRSW() const {
	return(rsw);
}

const RO::GND* RswObject::getGND() const {
	return(gnd);
}

void RswObject::getWorldPosition(int mapx, int mapy, float *rx, float *ry, float *rz) {
	float sizex = 0, sizey = 0;

	float tile = m_tilesize / 2;

	if (mapx < 0 || mapx >= (int)gat->getWidth())
		return;

	if (mapy < 0 || mapy >= (int)gat->getHeight())
		return;

	
	sizex = tile * gat->getWidth();
	sizey = tile * gat->getHeight();

	//const RO::GND::strCube& cube = gnd->getCube(mapx, mapy);
	const RO::GAT::strBlock& block = gat->getBlock(mapx, mapy);

	*rx = tile * mapx + m_tilesize / 2 - sizex / 2;
	*rz = tile * mapy + m_tilesize / 2 - sizey / 2;
	*ry = -(block.height[0] + block.height[1] + block.height[2] + block.height[3]) / 4;
}

bool RswObject::loadTextures(CacheManager& cache) {
	unsigned int i;
	sdle::Texture tex;
	std::string texname;

	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	for (i = 0; i < gnd->getTextureCount(); i++) {
		texname = "texture\\";
		texname += gnd->getTexture(i).path;
		tex = tm.Register(fm, texname);
		if (!tex.Valid()) {
			fprintf(stderr, "Warning: Texture not found: %s\n", texname);
		}
		textures.add(tex);
	}

	return(true);
}

RswObject* RswObject::open(CacheManager& cache, const char* map) {
	RO::RSW* rsw;

	// TODO: Delete active map (if any)

	std::string rsw_fn(map);
	
	rsw_fn += ".rsw";
	// Load the rsw object
	if (!cache.getROObjects().ReadRSW(rsw_fn.c_str(), cache.getFileManager())) {
		fprintf(stderr, "Error loading RSW file %s\n", rsw_fn.c_str());
		return(NULL);
	}
	rsw = (RO::RSW*)cache.getROObjects().get(rsw_fn);

	if (!cache.getROObjects().ReadGND(rsw->gnd_file, cache.getFileManager())) {
		fprintf(stderr, "Error loading GND file %s\n", rsw->gnd_file);
		return(NULL);
	}

	if (!cache.getROObjects().ReadGAT(rsw->gat_file, cache.getFileManager())) {
		fprintf(stderr, "Error loading GAT file %s\n", rsw->gat_file);
		return(NULL);
	}

	RswObject* obj = new RswObject(rsw, cache.getROObjects());
	obj->loadTextures(cache);

	/*
	printf("Map info:\n");
	printf("\tRequested map: %s\n", map);
	printf("\tGND Map size: %d, %d\n", obj->gnd->getWidth(), obj->gnd->getHeight());
	printf("\tGAT Map size: %d, %d\n", obj->gat->getWidth(), obj->gat->getHeight());
	*/
	
	return(obj);
}

void RswObject::DrawGND() {
	float sizex = 0, sizey = 0;
	unsigned int i, j;
	
	sizex = m_tilesize * gnd->getWidth();
	sizey = m_tilesize * gnd->getHeight();

	float rot[16];
	rot[0] = 1.0;
	rot[1] = 0.0;
	rot[2] = 0.0;
	rot[3] = 0.0;

	rot[4] = 0.0;
	rot[5] = 0.0;
	rot[6] = 1.0;
	rot[7] = 0.0;

	rot[8] = 0.0;
	rot[9] = -1.0;
	rot[10] = 0.0;
	rot[11] = 0.0;

	rot[12] = -sizex/2;
	rot[13] = 0;
	rot[14] = -sizey/2;
	rot[15] = 1.0;

	//Rotate 90 degrees about the z axis
	glMultMatrixf(rot);

	// Center the map
	//glTranslatef(-sizex/2, -sizey/2, 0);

	/*
	GLfloat Mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &Mat[0]);

	printf("[%.2f %.2f]\n", sizex/2,sizey/2);
	printf("[\n");
	printf("[%.2f %.2f %.2f %.2f]\n", Mat[0], Mat[1], Mat[2], Mat[3]);
	printf("[%.2f %.2f %.2f %.2f]\n", Mat[4], Mat[5], Mat[6], Mat[7]);
	printf("[%.2f %.2f %.2f %.2f]\n", Mat[8], Mat[9], Mat[10], Mat[11]);
	printf("[%.2f %.2f %.2f %.2f]\n", Mat[12], Mat[13], Mat[14], Mat[15]);
	printf("]\n");
	*/

	sdle::Texture tex;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	for (j = 0; j < gnd->getHeight(); j++) {
		for (i = 0; i < gnd->getWidth(); i++) {
			const RO::GND::strCube& cube = gnd->getCube(i, j);
			//cube = gnd->getCube(i, j);

			/* TILE UP */
			if (cube.tile_up != -1) {
				const RO::GND::strTile& tile = gnd->getTile(cube.tile_up);
				tex = textures[tile.texture_index];
				if (tex.Valid()) {
					tex.Activate();
					glBegin(GL_QUADS);
					glTexCoord2f(tile.texture_start[0],		1 - tile.texture_end[0]);
					glVertex3f(m_tilesize * i,		m_tilesize * j,		 cube.height[0]);

					glTexCoord2f(tile.texture_start[1],		1 - tile.texture_end[1]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * j,		 cube.height[1]);

					glTexCoord2f(tile.texture_start[3],		1 - tile.texture_end[3]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * (j + 1), cube.height[3]);

					glTexCoord2f(tile.texture_start[2],		1 - tile.texture_end[2]);
					glVertex3f(m_tilesize * i,		m_tilesize * (j + 1), cube.height[2]);
					glEnd();
				}
			}

			/* TILE SIDE */
			if (cube.tile_side != -1) {
				const RO::GND::strTile& tile = gnd->getTile(cube.tile_side);
				const RO::GND::strCube& cube2 = gnd->getCube(i, j+1);
				
				tex = textures[tile.texture_index];
				if (tex.Valid()) {
					tex.Activate();
					glBegin(GL_QUADS);
					glTexCoord2f(tile.texture_start[0],	tile.texture_end[0]);
					glVertex3f(m_tilesize * i,		m_tilesize * (j + 1), cube.height[2]);

					glTexCoord2f(tile.texture_start[1],	tile.texture_end[1]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * (j + 1), cube.height[3]);

					glTexCoord2f(tile.texture_start[3],	tile.texture_end[3]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * (j + 1), cube2.height[1]);

					glTexCoord2f(tile.texture_start[2],	tile.texture_end[2]);
					glVertex3f(m_tilesize * i,		m_tilesize * (j + 1), cube2.height[0]);
					glEnd();
				}
			}

			/* TILE ASIDE */
			if (cube.tile_aside != -1) {
				const RO::GND::strTile& tile = gnd->getTile(cube.tile_aside);
				const RO::GND::strCube& cube2 = gnd->getCube(i+1, j);
				tex = textures[tile.texture_index];
				if (tex.Valid()) {
					tex.Activate();
					glBegin(GL_QUADS);
					glTexCoord2f(tile.texture_start[0],	tile.texture_end[0]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * (j + 1), cube.height[3]);

					glTexCoord2f(tile.texture_start[1],	tile.texture_end[1]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * j, cube.height[1]);

					glTexCoord2f(tile.texture_start[3],	tile.texture_end[3]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * j, cube2.height[0]);

					glTexCoord2f(tile.texture_start[2],	tile.texture_end[2]);
					glVertex3f(m_tilesize * (i + 1),	m_tilesize * (j + 1), cube2.height[2]);
					glEnd();
				}
			}
		}
	}
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

void RswObject::DrawSelection(int mapx, int mapy) const {
#define ZOFFSET -0.002f

	float sizex = 0, sizey = 0;
	float tile = m_tilesize / 2;

	if (mapx < 0 || mapx >= (int)gat->getWidth())
		return;

	if (mapy < 0 || mapy >= (int)gat->getHeight())
		return;

	
	sizex = tile * gat->getWidth();
	sizey = tile * gat->getHeight();

	float rot[16];
	rot[0] = 1.0;
	rot[1] = 0.0;
	rot[2] = 0.0;
	rot[3] = 0.0;

	rot[4] = 0.0;
	rot[5] = 0.0;
	rot[6] = 1.0;
	rot[7] = 0.0;

	rot[8] = 0.0;
	rot[9] = -1.0;
	rot[10] = 0.0;
	rot[11] = 0.0;

	rot[12] = -sizex/2;
	rot[13] = 0;
	rot[14] = -sizey/2;
	rot[15] = 1.0;

	//Rotate 90 degrees about the z axis
	glPushMatrix();
	glMultMatrixf(rot);

	//const RO::GND::strCube& cube = gnd->getCube(mapx, mapy);
	const RO::GAT::strBlock& block = gat->getBlock(mapx, mapy);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,	0.0f); glVertex3f(tile * mapx,			tile * mapy,		block.height[0] + ZOFFSET);
	glTexCoord2f(1.0f,	0.0f); glVertex3f(tile * (mapx + 1),	tile * mapy,		block.height[1] + ZOFFSET);
	glTexCoord2f(1.0f,	1.0f); glVertex3f(tile * (mapx + 1),	tile * (mapy + 1),	block.height[3] + ZOFFSET);
	glTexCoord2f(0.0f,	1.0f); glVertex3f(tile * mapx,			tile * (mapy + 1),	block.height[2] + ZOFFSET);
	glEnd();

	glPopMatrix();
}


void RswObject::DrawRSW(int screen_x, int screen_y) {
	glPushMatrix();

	Draw();
	sdle::Vertex v;
	sdle::SDLEngine::getSingleton()->unProject(screen_x, screen_y, &v);

	// Our X and Y coordinates are inverted. Let's handle it right here.
	world_x = v.y;
	world_y = v.x;
	world_z = v.z;

	glPopMatrix();
}

void RswObject::Draw() {
	if (glIsList(gnd_gl)) {
		glCallList(gnd_gl);
	}
	else {
		gnd_gl = glGenLists(1);
		glNewList(gnd_gl, GL_COMPILE_AND_EXECUTE);
		DrawGND();
		glEndList();
	}
}

bool RswObject::isInFrustum(const Frustum&) const {
	return(true);
}

int RswObject::getMouseMapX() const {
	return((int)world_x / 5);
}

int RswObject::getMouseMapY() const {
	return((int)world_y / 5);
}

float RswObject::getWorldX() const { return(world_x); }
float RswObject::getWorldY() const { return(world_y); }
float RswObject::getWorldZ() const { return(world_z); }
