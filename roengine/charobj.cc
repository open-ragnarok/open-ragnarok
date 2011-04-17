#include "stdafx.h"

#include "roengine/objects/charobj.h"
#include "ro/common.h"

CharObj::CharObj() : Actor(CharType) {
}

CharObj::~CharObj() {
}

void CharObj::setAction(unsigned short action) {
//	m_action = action;
	m_act = action;

	m_bodyact.setAction(action);
	m_headact.setAction(action);

	m_bodyact.Play(!(m_act == 6 || m_act == 5));
	m_headact.Play(!(m_act == 6 || m_act == 5));

}

void CharObj::WalkTo(int x, int y) {
	setDest(x, y);
	setAction(1);
}

void CharObj::LookAt(int x, int y) {
	float dx = x - map_x;
	float dy = y - map_y;
//	float size = sqrt(dx * dx + dy * dy);
	float ax = 0, ay = 0;
	if (abs(dx) >= 0.1f)
		ax = (dx > 0.0f) ? 1 : -1;
	if (abs(dy) >= 0.1f)
		ay = (dy > 0.0f) ? 1 : -1;

	if (ax != 0.0f || ay != 0.0f)
		m_dir = ro::dir2Cdir(ax, ay);
}

void CharObj::Attack() {
//	int action = 5;

	// TODO: switch to current weapon

	// case no weapon
	setAction(10);

	m_headact.Play(false);
	m_bodyact.Play(false);

}

void CharObj::Damage() {
	setAction(6);

	m_headact.Play(false);
	m_bodyact.Play(false);
}

void CharObj::Dead() {
	setAction(8);
}

void CharObj::Draw() {
	if (m_map == NULL)
		return;

	if (!m_bodyact.valid())
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
		if (m_act == 0 || m_act == 4) {
		//	m_act = 1;
		//	setAction(1);
		}
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

//		moveticks += m_tickdelay;
	//	if (moveticks >= speed) {
//			moveticks = 0;
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
		//	m_act = 0;
			setAction(0);
		}
		else if ((m_act == 6 || m_act == 5 || m_act == 10) && !m_bodyact.isPlaying()) {
		//	m_act = 4;
			setAction(4);
		}
	}

	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	m_map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

//	m_bodyact.setAction(m_act);
//	m_headact.setAction(m_act);

	int dir = (cameraDir + 8 - m_dir) % 8;

	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_shadowact.Draw(m_tickdelay, ro::CDir::DIR_N, -0.1);
//	m_bodyact.Draw(m_tickdelay, (ro::CDir)dir, 0, !(m_act == 6 || m_act == 5)); // Draw
	m_bodyact.Draw(m_tickdelay, (ro::CDir)dir, 0); // Draw
	if (m_headact.valid())
		if (m_act == 0 || m_act == 2) // TODO: Fix
			m_headact.Draw(0, (ro::CDir)dir, 0.1);
		else
//			m_headact.Draw(m_tickdelay, (ro::CDir)dir, 0.1, !(m_act == 6 || m_act == 5));
			m_headact.Draw(m_tickdelay, (ro::CDir)dir, 0.1);
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

bool CharObj::valid() const {
	return(m_bodyact.valid());
}

bool CharObj::open(CacheManager& cache, ro::CJob job, ro::CSex sex, int hair) {
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

	// TODO: make this a parameter
	m_hair = hair;
	m_job = job;
	m_sex = sex;

	std::string act_n, spr_n;

	rogl::SprGL sprgl;

	char buf[256];
	// ========== Body
	sprintf(buf, "sprite\\%s\\%s\\%s\\%s_%s", ro::EUC::humans, ro::EUC::body, ro::EUC::sex[sex], ro::EUC::classname[job], ro::EUC::sex[sex]);

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
	m_bodyact.setAct((ro::ACT*)objects[act_n]);

	// ========== Head
	sprintf(buf, "sprite\\%s\\%s\\%s\\%d_%s", ro::EUC::humans, ro::EUC::head, ro::EUC::sex[sex], m_hair, ro::EUC::sex[sex]);

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
	m_headact.setSpr(sprgl);
	m_headact.setAct((ro::ACT*)objects[act_n]);
	m_headact.setExt(&m_bodyact);

	return(true);
}
