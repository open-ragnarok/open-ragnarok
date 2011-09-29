#include "stdafx.h"

#include "roengine/objects/npcobj.h"
#include "ro/common.h"

NpcObj::NpcObj() : Actor(NpcType) {
}

NpcObj::~NpcObj() {
}

void NpcObj::Draw() {
	if (m_map == NULL)
		return;

	if (!m_npc.valid())
		return;

	// Move...
	if (dest_x != map_x || dest_y != map_y) {
		if (m_act == 0) {
			m_act = 1;
		}
		float dx = dest_x - map_x;
		float dy = dest_y - map_y;
		float size = sqrt(dx * dx + dy * dy);

		m_dir = ro::dir2Cdir(dx, dy);

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

	m_npc.setAction(m_act);

	int dir;
	if (m_dir % 2 == 1) {// Odd
		if (m_dir < 4)
			dir = (cameraDir + 4 - m_dir) % 8;
		else if (m_dir > 4)
			dir = (cameraDir + 12 - m_dir) % 8;
	}
	else if (m_dir % 4 == 0)
		dir = (cameraDir + 4 + m_dir) % 8;
	else
		dir = (cameraDir + m_dir) % 8;

	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_shadowact.Draw(m_tickdelay, ro::DIR_N, -0.1);
	m_npc.Draw(m_tickdelay, (ro::CDir)dir); // Draw

	glTranslatef(0, 10, 0); // Moves our object to the proper place
	if (m_emotion > -1)
		m_emotionact.Draw(m_tickdelay, (ro::CDir)m_emotion, 0.2);
	if (!m_emotionact.isPlaying()) {
		m_emotion = -1;
	//	m_emotionact.setAction(1);
	//	m_emotionact.setAction(0);
		m_emotionact.Stop();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

bool NpcObj::valid() const {
	return(m_npc.valid());
}

bool NpcObj::open(CacheManager& cache, std::string name) {
	if (shadowLoaded) {
		openAct(cache, "sprite\\shadow", m_shadowact);
		shadowLoaded = true;
	}
	if (emotionLoaded) {
		char s[256];
		sprintf(s, "sprite\\%s\\emotion", ro::EUC::effects);
		openAct(cache, s, m_emotionact);
		emotionLoaded = true;
	}

	//Cache objects
	ROObjectCache& objects = cache.getROObjects();
	GLObjectCache& globjects = cache.getGLObjects();
	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	std::string act_n, spr_n;

	rogl::SprGL sprgl;

	char buf[256];
	// ========== Body
	sprintf(buf, "sprite\\npc\\%s", name.c_str());

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
	m_npc.setSpr(sprgl);
	m_npc.setAct((ro::ACT*)objects[act_n]);

	return(true);
}
