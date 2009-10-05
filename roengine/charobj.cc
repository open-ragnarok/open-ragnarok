#include "stdafx.h"

#include "roengine/objects/charobj.h"

CharObj::CharObj() : GLObject() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
	m_dir = RO::DIR_N;
}

CharObj::~CharObj() {
}

void CharObj::setMap(RswObject* map) {
	m_map = map;
}

void CharObj::Draw() {
	if (m_map == NULL)
		return;

	if (!m_bodyact.valid())
		return;

	// Move...
	if (dest_x != map_x || dest_y != map_y) {
		float dx = dest_x - map_x;
		float dy = dest_y - map_y;
		float size = sqrt(dx * dx + dy * dy);

		if (size < speed) {
			map_x = dest_x;
			map_y = dest_y;
		}
		else {
			dx /= size;
			dy /= size;

			dx *= speed;
			dy *= speed;
			map_x += dx;
			map_y += dy;
		}
	}

	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	m_map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

	int dir = 7 - cameraDir + m_dir;

	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_bodyact.Draw(m_tickdelay, (RO::CDir)dir); // Draw
	if (m_headact.valid())
		m_headact.Draw(m_tickdelay, (RO::CDir)dir);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

float CharObj::getPositionX() const {
	return(map_x);
}

float CharObj::getPositionY() const {
	return(map_y);
}

float CharObj::getDestinationX() const {
	return(dest_x);
}

float CharObj::getDestinationY() const {
	return(dest_y);
}


bool CharObj::valid() const {
	return(m_bodyact.valid());
}

void CharObj::setPos(float x, float y) {
	map_x = x;
	map_y = y;
	dest_x = x;
	dest_y = y;
}

void CharObj::setDest(float x, float y) {
	dest_x = x;
	dest_y = y;
}

bool CharObj::open(CacheManager& cache, RO::CJob job, RO::CSex sex) {
	// TODO: Clear in-use body, head, etc...
	//Cache objects
	ROObjectCache& objects = cache.getROObjects();
	GLObjectCache& globjects = cache.getGLObjects();
	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	// TODO: make this a parameter
	m_hair = 1;
	m_job = job;
	m_sex = sex;

	std::string act_n, spr_n;

	rogl::SprGL sprgl;

	char buf[256];
	// ========== Body
	sprintf(buf, "sprite\\%s\\%s\\%s\\%s_%s", RO::EUC::humans, RO::EUC::body, RO::EUC::sex[sex], RO::EUC::classname[job], RO::EUC::sex[sex]);

	// Setup filenames
	act_n = buf;
	spr_n = buf;
	act_n += ".act";
	spr_n += ".spr";

	// Reads the ACT object
	if (!objects.ReadACT(act_n, fm)) {
		fprintf(stderr, "Error loading act file %s.\n", act_n.c_str());
		return(false);
	}

	// Reads the SPR object
	if (!objects.ReadSPR(spr_n, fm)) {
		fprintf(stderr, "Error loading spr file %s.\n", spr_n.c_str());
		return(false);
	}

	// Converts the SPR object into a texture (SprGL)
	if (!sprgl.open((RO::SPR*)objects[spr_n])) {
		fprintf(stderr, "Error converting spr to texture.\n");
		return(false);
	}

	// Registers the SprGL Texture
	tm.Register(spr_n, sprgl.getTexture());

	// Creates an ActGL and registers it
	m_bodyact.setSpr(sprgl);
	m_bodyact.setAct((RO::ACT*)objects[act_n]);

	// ========== Head
	sprintf(buf, "sprite\\%s\\%s\\%s\\%d_%s", RO::EUC::humans, RO::EUC::head, RO::EUC::sex[sex], m_hair, RO::EUC::sex[sex]);

	// Setup filenames
	act_n = buf;
	spr_n = buf;
	act_n += ".act";
	spr_n += ".spr";

	// Reads the ACT object
	if (!objects.ReadACT(act_n, fm)) {
		fprintf(stderr, "Error loading act file %s.\n", act_n.c_str());
		return(false);
	}

	// Reads the SPR object
	if (!objects.ReadSPR(spr_n, fm)) {
		fprintf(stderr, "Error loading spr file %s.\n", spr_n.c_str());
		return(false);
	}

	// Converts the SPR object into a texture (SprGL)
	if (!sprgl.open((RO::SPR*)objects[spr_n])) {
		fprintf(stderr, "Error converting spr to texture.\n");
		return(false);
	}

	// Registers the SprGL Texture
	tm.Register(spr_n, sprgl.getTexture());

	// Creates an ActGL and registers it
	m_headact.setSpr(sprgl);
	m_headact.setAct((RO::ACT*)objects[act_n]);

	return(true);
}
