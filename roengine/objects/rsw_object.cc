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
	std::string gnd_fn = rsw->getGndFile();
	std::string gat_fn = rsw->getGatFile();
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


	for (unsigned int i = 0; i < rsw->getObjectCount(); i++) {
		const RO::RSW::ModelObject* rswobj = rsw->getModelObject(i);
		if (rswobj == NULL)
			continue;
		if (m_cache.getGLObjects().exists(rswobj->name)) {
			m_cache.getGLObjects().remove(rswobj->name);
		}
	}
}

const RO::RSW* RswObject::getRSW() const {
	return(rsw);
}

const RO::GND* RswObject::getGND() const {
	return(gnd);
}

void RswObject::getWorldPosition(float mapx, float mapy, float *rx, float *ry, float *rz) {
	float sizex = 0, sizey = 0;

	float tile = m_tilesize / 2;
	
	if (gat == NULL)
		return;

	if (mapx < 0 || mapx >= (int)gat->getWidth())
		return;

	if (mapy < 0 || mapy >= (int)gat->getHeight())
		return;

	
	sizex = tile * gat->getWidth();
	sizey = tile * gat->getHeight();

	unsigned int coordx = (unsigned int)mapx;
	unsigned int coordy = (unsigned int)mapy;


	int mx, my;		// The integer part of the map position
	float dx, dy;	// The decimal part of the map position
	dx = mapx;
	dy = (float)gat->getHeight() - mapy - 1;

	mx = (int)dx;
	my = (int)dy;

	dx -= mx;
	dy -= my;

	if (dx < 0.005f && dy < 0.005f) {
		*rx = tile * mx + tile / 2 - sizex / 2;
		*rz = tile * my + tile / 2 - sizey / 2;
		*ry = -gat->getAltitude(coordx, coordy);
	}
	else {
		float positions[4][3]; // Coordinates (0, 0), (1, 0), (1, 1), (0, 1)

		{
			positions[0][0] = tile * mx + tile / 2 - sizex / 2;
			positions[0][1] = -gat->getAltitude(coordx, coordy);
			positions[0][2] = tile * my + tile / 2 - sizey / 2;
		}
		{
			positions[1][0] = tile * (mx + 1) + tile / 2 - sizex / 2;
			positions[1][1] = -gat->getAltitude(coordx+1, coordy);
			positions[1][2] = tile * my + tile / 2 - sizey / 2;
		}
		{
			positions[2][0] = tile * (mx + 1) + tile / 2 - sizex / 2;
			positions[2][1] = -gat->getAltitude(coordx+1, coordy+1);
			positions[2][2] = tile * (my + 1) + tile / 2 - sizey / 2;
		}
		{
			positions[3][0] = tile * mx + tile / 2 - sizex / 2;
			positions[3][1] = -gat->getAltitude(coordx, coordy+1);
			positions[3][2] = tile * (my + 1) + tile / 2 - sizey / 2;
		}
		if (dx < 0.005f) {
			// Moving only along the "y"
			*rx = positions[0][0];
			*ry = positions[0][1] * (1 - dy) + positions[3][1] * dy;
			*rz = positions[0][2] * (1 - dy) + positions[3][2] * dy;
		}
		else if (dy < 0.005f) {
			// Moving only along the "x"
			*rx = positions[0][0] * (1 - dx) + positions[1][0] * dx;
			*ry = positions[0][1] * (1 - dx) + positions[1][1] * dx;
			*rz = positions[0][2];
		}
		else {
			*rx = positions[0][0] * (1 - dx) + positions[1][0] * dx;
			*rz = positions[0][2] * (1 - dy) + positions[3][2] * dy;
			/*                     2   3
			 *                     +---+
			 * upper triangle -->> |  /|
			 *                     | / |
			 *                     |/  | <<-- lower triangle
			 *                     +---+
			 *                     1   0
			 */

			int idx[4] = { 1, 0, 3, 2 };
			if (dx < (1 - dy)) {
				// Lower triangle
				// The height on (0, y)
				float hy = positions[idx[0]][1] + dy * (positions[idx[1]][1] - positions[idx[0]][1]);
				// The height on the diagonal in the coordinate y (1-y, y)
				float hp = positions[idx[3]][1] + dy * (positions[idx[1]][1] - positions[idx[3]][1]);
				// Proportion constant
				float maxx = 1 - dy;
				// The height
				float h = hy + (dx / maxx) * (hp - hy);

				*ry = h;
			}
			else {
				// Upper triangle
				// The height on (0, y)
				float hy = positions[idx[3]][1] + dy * (positions[idx[2]][1] - positions[idx[3]][1]);
				// The height on the diagonal in the coordinate y (1-y, y)
				float hp = positions[idx[3]][1] + dy * (positions[idx[1]][1] - positions[idx[3]][1]);
				// Proportion constant
				float minx = 1 - dy;

				float k = dx - minx;
				float j = k / dy;

				// The height
				float h = hy + j * (hp - hy);

				*ry = h;
			}
		}
	}
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
			fprintf(stderr, "Warning: Texture not found: %s\n", texname.c_str());
		}
		textures.add(tex);
	}

	// Load water
	char waterfn[128];
	for (i = 0; i <= 31; i++) {
		sprintf(waterfn, "texture\\%s\\water%d%02d.jpg", RO::EUC::water, rsw->getWater().type, i);
		tex = tm.RegisterJPEG(fm, waterfn);
		water_tex.add(tex);
	}

	return(true);
}

RswObject* RswObject::open(CacheManager& cache, const char* map) {
	RO::RSW* rsw;

	// TODO: Show loading screen
	printf("Loading map %s\n",map);

	// TODO: Delete active map (if any)

	std::string rsw_fn(map);
	
	rsw_fn += ".rsw";
	// Load the rsw object
	if (!cache.getROObjects().exists(rsw_fn)) {
		if (!cache.getROObjects().ReadRSW(rsw_fn.c_str(), cache.getFileManager())) {
			fprintf(stderr, "Error loading RSW file %s\n", rsw_fn.c_str());
			return(NULL);
		}
	}
	rsw = (RO::RSW*)cache.getROObjects().get(rsw_fn);

	if (!cache.getROObjects().exists(rsw->getGndFile())) {
		if (!cache.getROObjects().ReadGND(rsw->getGndFile(), cache.getFileManager())) {
			fprintf(stderr, "Error loading GND file %s\n", rsw->getGndFile());
			return(NULL);
		}
	}

	if (!cache.getROObjects().exists(rsw->getGatFile())) {
		if (!cache.getROObjects().ReadGAT(rsw->getGatFile(), cache.getFileManager())) {
			fprintf(stderr, "Error loading GAT file %s\n", rsw->getGatFile());
			return(NULL);
		}
	}

	RswObject* obj = new RswObject(rsw, cache);
	obj->loadTextures(cache);

	// Load objects
	RO::RSM* rsm;
	RsmObject* rsmobject;
	char fn[128];
	unsigned int i;

	float xoffset = m_tilesize * obj->gnd->getWidth() / 2;
	float yoffset = m_tilesize * obj->gnd->getHeight() / 2;

	for (i = 0; i < rsw->getObjectCount(); i++) {
		const RO::RSW::ModelObject* rswobj = rsw->getModelObject(i);
		if (rswobj == NULL)
			continue;

		sprintf(fn, "model\\%s", rswobj->modelName);
		if (!cache.getROObjects().exists(fn)) {
			if (!cache.getROObjects().ReadRSM(fn, cache.getFileManager())) {
				fprintf(stderr, "Error loading RSM file %s.\n", fn);
				continue;
			}
		}
		rsm = (RO::RSM*)cache.getROObjects()[fn];
		rsmobject = new RsmObject(rsm, rswobj);
		rsmobject->loadTextures(cache);
		float x = rsmobject->getPos().getX() + xoffset;
		float y = rsmobject->getPos().getY();
		float z = rsmobject->getPos().getZ() + yoffset;

		rsmobject->setPos(x, y, z);

		cache.getGLObjects().add(rswobj->name, rsmobject);
	}

	// TODO: Hide loading screen
	printf("Map %s loaded\n",map);
	
	return(obj);
}

void RswObject::getRot(float sizex, float sizey, float rot[16]) {
	//Rotate -90 degrees about the z axis

	rot[0] = 1.0;
	rot[1] = 0.0;
	rot[2] = 0.0;
	rot[3] = 0.0;

	rot[4] = 0.0;
	rot[5] = 1.0;
	rot[6] = 0.0;
	rot[7] = 0.0;

	rot[8] = 0.0;
	rot[9] = 0.0;
	rot[10] = -1.0;
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
					float texcoords[4][2] = {
						{ tile.texture_start[0], 1.0f - tile.texture_end[0]},
						{ tile.texture_start[1], 1.0f - tile.texture_end[1]},
						{ tile.texture_start[3], 1.0f - tile.texture_end[3]},
						{ tile.texture_start[2], 1.0f - tile.texture_end[2]}
					};
					glBegin(GL_QUADS);
					glTexCoord2fv(texcoords[0]); glVertex3f(m_tilesize * i,			-cube.height[0], m_tilesize * j);
					glTexCoord2fv(texcoords[1]); glVertex3f(m_tilesize * (i + 1),	-cube.height[1], m_tilesize * j);
					glTexCoord2fv(texcoords[2]); glVertex3f(m_tilesize * (i + 1),	-cube.height[3], m_tilesize * (j + 1));
					glTexCoord2fv(texcoords[3]); glVertex3f(m_tilesize * i,			-cube.height[2], m_tilesize * (j + 1));
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
					float texcoords[4][2] = {
						{ tile.texture_start[2], tile.texture_end[2]},
						{ tile.texture_start[3], tile.texture_end[3]},
						{ tile.texture_start[1], tile.texture_end[1]},
						{ tile.texture_start[0], tile.texture_end[0]}
					};
					glBegin(GL_QUADS);
					glTexCoord2fv(texcoords[0]); glVertex3f(m_tilesize * i,			-cube.height[2],	m_tilesize * (j + 1));
					glTexCoord2fv(texcoords[1]); glVertex3f(m_tilesize * (i + 1),	-cube.height[3],	m_tilesize * (j + 1));
					glTexCoord2fv(texcoords[2]); glVertex3f(m_tilesize * (i + 1),	-cube2.height[1],	m_tilesize * (j + 1));
					glTexCoord2fv(texcoords[3]); glVertex3f(m_tilesize * i,			-cube2.height[0],	m_tilesize * (j + 1));
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
					float texcoords[4][2] = {
						{ tile.texture_start[2], tile.texture_end[2]},
						{ tile.texture_start[3], tile.texture_end[3]},
						{ tile.texture_start[1], tile.texture_end[1]},
						{ tile.texture_start[0], tile.texture_end[0]}
					};

					glBegin(GL_QUADS);
					glTexCoord2fv(texcoords[0]); glVertex3f(m_tilesize * (i + 1),	-cube.height[3],	m_tilesize * (j + 1));
					glTexCoord2fv(texcoords[1]); glVertex3f(m_tilesize * (i + 1),	-cube.height[1],	m_tilesize * j);
					glTexCoord2fv(texcoords[2]); glVertex3f(m_tilesize * (i + 1),	-cube2.height[0],	m_tilesize * j);
					glTexCoord2fv(texcoords[3]); glVertex3f(m_tilesize * (i + 1),	-cube2.height[2],	m_tilesize * (j + 1));
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
	unsigned int cycle = rsw->getWater().animSpeed * 100;
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

	float waterh = (float)rsw->getWater().level;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < gnd->getWidth() / WATER_MULTIPLIER; i++) {
		for (unsigned int j = 0; j < gnd->getHeight() / WATER_MULTIPLIER; j++) {
			glTexCoord2f(0.0f, 0.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * i,			-waterh, WATER_MULTIPLIER * m_tilesize * j);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * (i + 1),	-waterh, WATER_MULTIPLIER * m_tilesize * j);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * (i + 1),	-waterh, WATER_MULTIPLIER * m_tilesize * (j + 1));
			glTexCoord2f(0.0f, 1.0f); glVertex3f(WATER_MULTIPLIER * m_tilesize * i,			-waterh, WATER_MULTIPLIER * m_tilesize * (j + 1));
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();
}

void RswObject::DrawSelection(int mapx, int mapy) const {
#define HOFFSET +0.1f

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
	const RO::GAT::Cell& cell = gat->getCell(mapx, mapy);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,	0.0f); glVertex3f(tile * mapx,			-cell.height[0] + HOFFSET, tile * mapy);
	glTexCoord2f(1.0f,	0.0f); glVertex3f(tile * (mapx + 1),	-cell.height[1] + HOFFSET, tile * mapy);
	glTexCoord2f(1.0f,	1.0f); glVertex3f(tile * (mapx + 1),	-cell.height[3] + HOFFSET, tile * (mapy + 1));
	glTexCoord2f(0.0f,	1.0f); glVertex3f(tile * mapx,			-cell.height[2] + HOFFSET, tile * (mapy + 1));
	glEnd();

	glPopMatrix();
}


void RswObject::setMouse(int screen_x, int screen_y) {
	mouse_x = screen_x;
	mouse_y = screen_y;
}

void RswObject::DrawObjects() {
	unsigned int i = 0;
	float sizex, sizey;
	GLObjectCache& cache = m_cache.getGLObjects();

	sizex = m_tilesize * gnd->getWidth();
	sizey = m_tilesize * gnd->getHeight();

	glPushMatrix();
	//glTranslatef(sizex / 2, 0, sizey / 2);
	//Frustum f;
	//f.Calculate();

	for (i = 0; i < rsw->getObjectCount(); i++) {
		const RO::RSW::ModelObject* rswobj = rsw->getModelObject(i);
		if (rswobj == NULL)
			continue;

		if (cache.exists(rswobj->name)) {
			//cache[rswobj->name]->Render(m_tickdelay, &f);
			cache[rswobj->name]->Render(m_tickdelay, m_frustum);
		}
	}

	glPopMatrix();
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
	DrawObjects();

	sdle::Vertex v;
	sdle::SDLEngine::getSingleton()->unProject(mouse_x, mouse_y, &v);

	world_x = v.x;
	world_y = v.y;
	world_z = v.z;
	glPopMatrix();
}

bool RswObject::isInFrustum(const Frustum&) const {
	return(true);
}

int RswObject::getMouseMapX() const {
	return((int)world_x / 5);
}

int RswObject::getMouseMapY() const {
	return((int)world_z / 5);
}

float RswObject::getWorldX() const { return(world_x); }
float RswObject::getWorldY() const { return(world_y); }
float RswObject::getWorldZ() const { return(world_z); }
