/* $Id$ */
#include "stdafx.h"

#include "roengine/objects/rsw_object.h"
#include "sdle/sdl_engine.h"
#include "sdle/texture_jpeg.h"

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

float RswObject::m_tilesize = 10.0f;

RswObject::RswObject(const ro::RSW* rsw, CacheManager& c) : GLObject(), m_cache(c) {

	ROObjectCache& cache = m_cache.getROObjects();

	this->rsw = rsw;
	std::string gnd_fn = rsw->getGndFile();
	std::string gat_fn = rsw->getGatFile();
	this->gnd = (ro::GND*)cache[gnd_fn];
	this->gat = (ro::GAT*)cache[gat_fn];

	gnd_gl = 0;

	m_waterframe = 0;
	m_waterdelay = 0;
	m_waveOffset = 0;
}

RswObject::~RswObject() {
	if (glIsList(gnd_gl))
		glDeleteLists(gnd_gl, 1);


	for (unsigned int i = 0; i < rsw->getObjectCount(); i++) {
		const ro::RSW::ModelObject* rswobj = rsw->getModelObject(i);
		if (rswobj == NULL)
			continue;
		if (m_cache.getGLObjects().exists(rswobj->name)) {
			m_cache.getGLObjects().remove(rswobj->name);
		}
	}
}

const ro::RSW* RswObject::getRSW() const {
	return(rsw);
}

const ro::GND* RswObject::getGND() const {
	return(gnd);
}

void RswObject::getWorldPosition(float mapx, float mapy, float *rx, float *ry, float *rz) {
	if (gat == NULL)
		return;

	float centerx = gat->getWidth() * 0.5f;
	float centery = gat->getHeight() * 0.5f;
	float zoom = m_tilesize * 0.5f;// zoom factor of a gat file?

	if (rx != NULL) *rx = zoom * (mapx - centerx + 0.5f);
	if (ry != NULL) *ry = -gat->getAltitude(mapx, mapy);
	if (rz != NULL) *rz = zoom * (gat->getHeight() - mapy - 1 - centery + 0.5f);
}

bool RswObject::loadTextures(CacheManager& cache) {
	unsigned int i;
	sdle::Texture tex;
	std::string texname;

	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	for (i = 0; i < gnd->getTextureCount(); i++) {
		texname = "texture\\";
		texname += gnd->getTexture(i);
		tex = tm.Register(fm, texname);
		if (!tex.Valid()) {
			fprintf(stderr, "Warning: Texture not found: %s\n", texname.c_str());
		}
		textures.add(tex);
	}

	// Load water
	char waterfn[128];
	for (i = 0; i <= 31; i++) {
		sprintf(waterfn, "texture\\%s\\water%d%02d.jpg", ro::EUC::water, rsw->getWater().type, i);
		tex = tm.RegisterJPEG(fm, waterfn);
		water_tex.add(tex);
	}

	return(true);
}

RswObject* RswObject::open(CacheManager& cache, const char* map) {
	ro::RSW* rsw;

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
	rsw = (ro::RSW*)cache.getROObjects().get(rsw_fn);

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
	ro::RSM* rsm;
	RsmObject* rsmobject;
	char fn[128];
	unsigned int i;

	float xoffset = m_tilesize * obj->gnd->getWidth() / 2;
	float yoffset = m_tilesize * obj->gnd->getHeight() / 2;

	for (i = 0; i < rsw->getObjectCount(); i++) {
		const ro::RSW::ModelObject* rswobj = rsw->getModelObject(i);
		if (rswobj == NULL)
			continue;

		sprintf(fn, "model\\%s", rswobj->modelName);
		if (!cache.getROObjects().exists(fn)) {
			if (!cache.getROObjects().ReadRSM(fn, cache.getFileManager())) {
				fprintf(stderr, "Error loading RSM file %s.\n", fn);
				continue;
			}
		}
		rsm = (ro::RSM*)cache.getROObjects()[fn];
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

void RswObject::DrawSurface(const ro::GND::Surface& surface, const float* vertices) {
	// TODO lightmap (use multi-texture extension?)
	// TODO what should happen with the color? should glEnable(GL_COLOR_MATERIAL) be used?
	//glColor4ub(surface.color.r, surface.color.g, surface.color.b, surface.color.a);
	if (surface.textureId != -1 && textures[surface.textureId].Valid())
	{
		const sdle::Texture texture = textures[surface.textureId];
		float tvertices[4 * 2] = {
			surface.u[0] * texture.getMaxU(), (1.0f - surface.v[0]) * texture.getMaxV(),
			surface.u[1] * texture.getMaxU(), (1.0f - surface.v[1]) * texture.getMaxV(),
			surface.u[2] * texture.getMaxU(), (1.0f - surface.v[2]) * texture.getMaxV(),
			surface.u[3] * texture.getMaxU(), (1.0f - surface.v[3]) * texture.getMaxV()
		};
		glEnable(GL_TEXTURE_2D);
		texture.Activate();
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2fv(tvertices + 0); glVertex3fv(vertices + 0);
		glTexCoord2fv(tvertices + 2); glVertex3fv(vertices + 3);
		glTexCoord2fv(tvertices + 4); glVertex3fv(vertices + 6);
		glTexCoord2fv(tvertices + 6); glVertex3fv(vertices + 9);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(vertices + 0);
		glVertex3fv(vertices + 3);
		glVertex3fv(vertices + 6);
		glVertex3fv(vertices + 9);
		glEnd();
	}
}

void RswObject::DrawGND() {
	unsigned int cellx, celly;

	float rot[16];
	float sizex = gnd->getZoom() * gnd->getWidth();
	float sizey = gnd->getZoom() * gnd->getHeight();
	getRot(sizex, sizey, rot);
	glMultMatrixf(rot);// TODO should this really be here?

	glPushMatrix();
	glScalef(gnd->getZoom(), -1, gnd->getZoom());
	glEnable(GL_ALPHA_TEST); // TODO why is this needed?
	for (celly = 0; celly < gnd->getHeight(); celly++) {
		for (cellx = 0; cellx < gnd->getWidth(); cellx++) {
			const ro::GND::Cell& cell = gnd->getCell(cellx, celly);
			if( cell.topSurfaceId != -1 )
			{
				const ro::GND::Surface& surface = gnd->getSurface(cell.topSurfaceId);
				float vertices[4 * 3] = {
					(float)(cellx    ), cell.height[0], (float)(celly    ),
					(float)(cellx + 1), cell.height[1], (float)(celly    ),
					(float)(cellx    ), cell.height[2], (float)(celly + 1),
					(float)(cellx + 1), cell.height[3], (float)(celly + 1)
				};
				DrawSurface(surface, vertices);
			}
			if( cell.frontSurfaceId != -1 )
			{
				const ro::GND::Surface& surface = gnd->getSurface(cell.frontSurfaceId);
				const ro::GND::Cell& cell2 = gnd->getCell(cellx, celly + 1);// north
				float vertices[4 * 3] = {
					(float)(cellx    ), cell.height[2] , (float)(celly + 1),
					(float)(cellx + 1), cell.height[3] , (float)(celly + 1),
					(float)(cellx    ), cell2.height[0], (float)(celly + 1),
					(float)(cellx + 1), cell2.height[1], (float)(celly + 1)
				};
				DrawSurface(surface, vertices);
			}
			if( cell.rightSurfaceId != -1 )
			{
				const ro::GND::Surface& surface = gnd->getSurface(cell.rightSurfaceId);
				const ro::GND::Cell& cell2 = gnd->getCell(cellx + 1, celly);// east
				float vertices[4 * 3] = {
					(float)(cellx + 1), cell.height[3] , (float)(celly + 1),
					(float)(cellx + 1), cell.height[1] , (float)(celly    ),
					(float)(cellx + 1), cell2.height[2], (float)(celly + 1),
					(float)(cellx + 1), cell2.height[0], (float)(celly    )
				};
				DrawSurface(surface, vertices);
			}
		}
	}
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void RswObject::DrawWater() {
#define WATERFPS 45 // TODO check how the speed works in original
	const ro::RSW::Water& water = rsw->getWater();
	unsigned int cycle = water.animSpeed * 1000 / WATERFPS;
	m_waterdelay = (m_waterdelay + m_tickdelay) % (cycle * 32);
	m_waterframe = m_waterdelay / cycle;
	m_waveOffset += m_tickdelay * water.waveSpeed * WATERFPS / 1000;
	while (m_waveOffset > 180.0)
		m_waveOffset -= 360;

	glPushMatrix();
	glScalef(gnd->getZoom(), -1, gnd->getZoom());

	// TODO set water opacity/alpha to 0x90
	glEnable(GL_TEXTURE_2D);
	water_tex[m_waterframe].Activate();
	for (unsigned int y = 0; y < gnd->getHeight(); y++) {
		for (unsigned int x = 0; x < gnd->getWidth(); x++) {
			float height[3];
			if( water.waveHeight == 0.0 )
			{
				height[0] = height[1] = height[2] = water.level;
			}
			else
			{
				height[0] = (float)sin(((x + y    ) * water.wavePitch + m_waveOffset) * M_PI / 180.0) * water.waveHeight + water.level;
				height[1] = (float)sin(((x + y - 1) * water.wavePitch + m_waveOffset) * M_PI / 180.0) * water.waveHeight + water.level;
				height[2] = (float)sin(((x + y + 1) * water.wavePitch + m_waveOffset) * M_PI / 180.0) * water.waveHeight + water.level;
			}
			const ro::GND::Cell& cell = gnd->getCell(x, y);
			if (cell.height[0] <= height[0] && cell.height[1] <= height[0] && cell.height[2] <= height[0] && cell.height[3] <= height[0])
				continue;// water is below the ground
			// TODO exclude tiles based on camera frustum?
			// TODO check texture orientation in original
			float u1 = (x % 4    ) * 0.25f;
			float u2 = (x % 4 + 1) * 0.25f;
			float v1 = (y % 4    ) * 0.25f;
			float v2 = (y % 4 + 1) * 0.25f;
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(u1, v1); glVertex3f((float)(x    ), height[1], (float)(y    ));
			glTexCoord2f(u2, v1); glVertex3f((float)(x + 1), height[0], (float)(y    ));
			glTexCoord2f(u1, v2); glVertex3f((float)(x    ), height[0], (float)(y + 1));
			glTexCoord2f(u2, v2); glVertex3f((float)(x + 1), height[2], (float)(y + 1));
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
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

	//const ro::GND::strCube& cube = gnd->getCube(mapx, mapy);
	const ro::GAT::Cell& cell = gat->getCell(mapx, mapy);

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
		const ro::RSW::ModelObject* rswobj = rsw->getModelObject(i);
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
