#include "stdafx.h"

#include "roengine/objects/mobobj.h"
#include "ro/common.h"

MobObj::MobObj() : Actor(MobType) {
}

MobObj::~MobObj() {
}

void MobObj::Draw() {
	if (m_map == NULL)
		return;

	if (!m_mob.valid())
		return;

	// Move...
/*	if (dest_x != map_x || dest_y != map_y) {
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
	}*/
	if (dest_x != map_x || dest_y != map_y) {
	//	if (m_act == 0) {
			m_act = 1;
	//	}
		float dx = dest_x - map_x;
		float dy = dest_y - map_y;
	//	float size = sqrt(dx * dx + dy * dy);
		float ax = 0, ay = 0;
		if (abs(dx) >= 0.1f)
			ax = (dx > 0.0f) ? 1 : -1;
		if (abs(dy) >= 0.1f)
			ay = (dy > 0.0f) ? 1 : -1;


		if (ax != 0.0f || ay != 0.0f)
			m_dir = ro::dir2Cdir(ax, ay);

	//	moveticks += m_tickdelay;
	//	if (moveticks >= speed) {
	//		moveticks = 0;
			float m = 1.0f / ((float)speed / m_tickdelay);
			if (abs(dx) >= 0.1f && abs(dy) >= 0.1f)
				m *= 0.75f;
			if (abs(dx) >= m) {
				if (dest_x > map_x) {
					map_x += m;
				}
				else if (dest_x < map_x) {
					map_x -= m;
				}
			} else
				map_x = dest_x;

			if (abs(dy) >= m) {
				if (dest_y > map_y) {
					map_y += m;
				}
				else if (dest_y < map_y) {
					map_y -= m;
				}
			} else
				map_y = dest_y;
	//	}
	}
	else {
		if (m_act == 1) {
			m_act = 0;
		}
		else if ((m_act == 2 || m_act == 3) && !m_mob.isPlaying()) {
			m_act = 0;
		}
	}

	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	m_map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

	m_mob.setAction(m_act);

//	int dir = (cameraDir + m_dir) % 8;
	int dir = (cameraDir + 8 - m_dir) % 8;

	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_shadowact.Draw(m_tickdelay, ro::CDir::DIR_N, -0.1);
	m_mob.Draw(m_tickdelay, (ro::CDir)dir, !(m_act == 2 || m_act == 3)); // Draw
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

bool MobObj::valid() const {
	return(m_mob.valid());
}

bool MobObj::open(CacheManager& cache, std::string name) {
	if (shadowLoaded) {
		openAct(cache, "sprite\\shadow", m_shadowact);
		shadowLoaded = true;
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
	sprintf(buf, "sprite\\%s\\%s", ro::EUC::monsters, name.c_str());

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
	m_mob.setSpr(sprgl);
	m_mob.setAct((ro::ACT*)objects[act_n]);

	return(true);
}
