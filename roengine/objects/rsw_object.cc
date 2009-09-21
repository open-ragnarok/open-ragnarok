/* $Id$ */
#include "stdafx.h"

#include "roengine/objects/rsw_object.h"
#include "sdle/sdl_engine.h"
#include "sdle/texture_jpeg.h"

#include <GL/gl.h>
#include <GL/glu.h>

float RswObject::m_tilesize = 10.0f;

RswObject::RswObject(const RO::RSW* rsw, CacheManager& c) : GLObject(), m_cache(c) {

	ROObjectCache& cache = m_cache.getROObjects();

	this->rsw = rsw;
	std::string gnd_fn = rsw->gnd_file;
	std::string gat_fn = rsw->gat_file;
	this->gnd = (RO::GND*)cache[gnd_fn];
	this->gat = (RO::GAT*)cache[gat_fn];

	m_watergl = 0;
	gnd_gl = 0;

	m_waterframe = 0;
	m_waterdelay = 0;
}

RswObject::~RswObject() {
	if (glIsList(m_watergl))
		glDeleteLists(m_watergl, 1);
	if (glIsList(gnd_gl))
		glDeleteLists(gnd_gl, 1);
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

	int mx, my;
	mx = mapx;
	my = gat->getHeight() - mapy - 1;

	//const RO::GND::strCube& cube = gnd->getCube(mapx, mapy);
	const RO::GAT::strBlock& block = gat->getBlock(mapx, mapy);

	*rx = tile * mx + tile / 2 - sizex / 2;
	*rz = tile * my + tile / 2 - sizey / 2;
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

	// Load water
	char waterfn[128];
	for (i = 0; i <= 31; i++) {
		sprintf(waterfn, "texture\\%s\\water%d%02d.jpg", RO::EUC::water, rsw->water.type, i);
		tex = tm.RegisterJPEG(fm, waterfn);
		water_tex.add(tex);
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

	RswObject* obj = new RswObject(rsw, cache);
	obj->loadTextures(cache);

	// Load objects
	RO::RSM* rsm;
	RO::RSW::Model* rswobj;
	RsmObject* rsmobject;
	char fn[128];
	unsigned int i;
	for (i = 0; i < rsw->getObjectCount(); i++) {
		if (!rsw->getObject(i)->isType(RO::RSW::OT_Model))
			continue;

		rswobj = (RO::RSW::Model*)rsw->getObject(i);
		sprintf(fn, "model\\%s", rswobj->data->filename);
		if (!cache.getROObjects().exists(fn)) {
			if (!cache.getROObjects().ReadRSM(fn, cache.getFileManager())) {
				fprintf(stderr, "Error loading RSM file %s.\n", fn);
				continue;
			}
		}
		rsm = (RO::RSM*)cache.getROObjects()[fn];
		rsmobject = new RsmObject(rsm, rswobj);
		rsmobject->loadTextures(cache);

		cache.getGLObjects().add(rswobj->getName(), rsmobject);
	}
	
	return(obj);
}

void RswObject::getRot(float sizex, float sizey, float rot[16]) {
	//Rotate -90 degrees about the z axis

	rot[0] = 1.0;
	rot[1] = 0.0;
	rot[2] = 0.0;
	rot[3] = 0.0;

	rot[4] = 0.0;
	rot[5] = 0.0;
	rot[6] = -1.0;
	rot[7] = 0.0;

	rot[8] = 0.0;
	rot[9] = -1.0;
	rot[10] = 0.0;
	rot[11] = 0.0;

	rot[12] = -sizex/2;
	rot[13] = 0;
	rot[14] = sizey/2;
	rot[15] = 1.0;
}

void RswObject::DrawGND() {
	float sizex = 0, sizey = 0;
	unsigned int i, j;
	
	sizex = m_tilesize * gnd->getWidth();
	sizey = m_tilesize * gnd->getHeight();

	float rot[16];
	getRot(sizex, sizey, rot);

	glMultMatrixf(rot);

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

void RswObject::DrawWater() {
#define WATER_MULTIPLIER 4
	m_waterdelay += m_tickdelay;
	int cycle = rsw->water.texture_cycling * 100;
	while (m_waterdelay > cycle) {
		m_waterdelay -= cycle;
		m_waterframe++;
		if (m_waterframe > 31) {
			m_waterframe = 0;
		}
	}
	water_tex[m_waterframe].Activate();

	if (glIsList(m_watergl)) {
		glCallList(m_watergl);
		return;
	}

	m_watergl = glGenLists(1);
	glNewList(m_watergl, GL_COMPILE_AND_EXECUTE);

	float waterh = rsw->water.height;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < gnd->getWidth() / WATER_MULTIPLIER; i++) {
		for (unsigned int j = 0; j < gnd->getHeight() / WATER_MULTIPLIER; j++) {
			glTexCoord2f(0.0f, 0.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * i,			WATER_MULTIPLIER * m_tilesize * j,		 waterh);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * (i + 1),	WATER_MULTIPLIER * m_tilesize * j,		 waterh);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * (i + 1),	WATER_MULTIPLIER * m_tilesize * (j + 1), waterh);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * i,			WATER_MULTIPLIER * m_tilesize * (j + 1), waterh);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();
}

void RswObject::DrawSelection(int mapx, int mapy) const {
#define ZOFFSET -0.1f

	float sizex = 0, sizey = 0;
	float tile = m_tilesize / 2;

	if (mapx < 0 || mapx >= (int)gat->getWidth())
		return;

	if (mapy < 0 || mapy >= (int)gat->getHeight())
		return;

	
	sizex = tile * gat->getWidth();
	sizey = tile * gat->getHeight();

	float rot[16];
	getRot(sizex, sizey, rot);

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


void RswObject::setMouse(int screen_x, int screen_y) {
	mouse_x = screen_x;
	mouse_y = screen_y;
}

void RswObject::DrawObjects() {
	unsigned int i = 0;
	RO::RSW::Model* rswobj;
	GLObjectCache& cache = m_cache.getGLObjects();
	for (i = 0; i < rsw->getObjectCount(); i++) {
		if (!rsw->getObject(i)->isType(RO::RSW::OT_Model))
			continue;

		rswobj = (RO::RSW::Model*)rsw->getObject(i);

		if (cache.exists(rswobj->data->m_name)) {
			cache[rswobj->data->m_name]->Render(m_tickdelay, m_frustum);
		}
	}
}

void RswObject::Draw() {
	glPushMatrix();
	if (glIsList(gnd_gl)) {
		glCallList(gnd_gl);
	}
	else {
		gnd_gl = glGenLists(1);
		glNewList(gnd_gl, GL_COMPILE_AND_EXECUTE);
		DrawGND();
		glEndList();
	}

	DrawWater();

	sdle::Vertex v;
	sdle::SDLEngine::getSingleton()->unProject(mouse_x, mouse_y, &v);

	world_x = v.x;
	world_y = v.y;
	world_z = v.z;
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	DrawObjects();
	glPopMatrix();
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
