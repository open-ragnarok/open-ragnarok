#include "stdafx.h"

#include "roengine/objects/charobj.h"
#include "ro/common.h"

CharObj::CharObj() : Actor() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
	m_dir = RO::DIR_N;
}

CharObj::~CharObj() {
}

void CharObj::Draw() {
	if (m_map == NULL)
		return;

	if (!m_bodyact.valid())
		return;

	// Move...
	if (dest_x != map_x || dest_y != map_y) {
		if (m_act == 0) {
			m_act = 1;
		}
		float dx = dest_x - map_x;
		float dy = dest_y - map_y;
		float size = sqrt(dx * dx + dy * dy);

		m_dir = RO::dir2Cdir(dx, dy);

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
	else {
		if (m_act == 1) {
			m_act = 0;
		}
	}

	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	m_map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

	m_bodyact.setAction(m_act);
	m_headact.setAction(m_act);

	int dir = 7 - cameraDir + m_dir;
	if (dir >= 8)
		dir -= 8;

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

bool CharObj::valid() const {
	return(m_bodyact.valid());
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
	if (!cache.getSprGLObjects().exists(spr_n)) {
		if (!cache.getSprGLObjects().Load(spr_n, objects, fm)) {
			fprintf(stderr, "Error opening SPR file");
			return(false);
		}
	}

	sprgl = *cache.getSprGLObjects().get(spr_n);

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
	if (!cache.getSprGLObjects().exists(spr_n)) {
		if (!cache.getSprGLObjects().Load(spr_n, objects, fm)) {
			fprintf(stderr, "Error opening SPR file");
			return(false);
		}
	}

	// Creates an ActGL and registers it
	m_headact.setSpr(sprgl);
	m_headact.setAct((RO::ACT*)objects[act_n]);

	return(true);
}
