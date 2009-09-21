#include "stdafx.h"

#include "roengine/objects/charobj.h"

CharObj::CharObj() : GLObject() {
	map_x = map_y = 0;
	m_act = 0;
	m_frame = 0;
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
	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	m_map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_bodyact.Draw(m_tickdelay); // Draw
	if (m_headact.valid())
		m_headact.Draw(m_tickdelay);
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
