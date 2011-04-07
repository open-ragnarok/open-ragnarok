/* $Id$ */
#include "stdafx.h"

#include <math.h>
#include "rogl/actgl.h"

namespace rogl {

ActGL::ActGL() {
	act = NULL;
	ext = NULL;
	m_action = 0;
	m_delay = 0;
	m_frame = 0;
	m_playing = false;
	m_loop = true;
}

ActGL::ActGL(const ActGL& _act) {
	*this = _act;
}

ActGL::~ActGL() {
}

void ActGL::setAction(unsigned short action) {
	if (m_action != action)
	{
		m_delay = 0;
		m_frame = 0;
		m_action = action;
	}
}

bool ActGL::isPlaying() {
	return m_playing;
}

void ActGL::Play(bool loop) {
	m_playing = true;
	m_loop = loop;
}

void ActGL::Stop(bool reset) {
	m_playing = false;
	if (reset) {
		m_delay = 0;
		m_frame = 0;
	}
}

bool ActGL::valid() const {
	return(act != NULL);
}


void ActGL::Draw() const {
}


void ActGL::Draw(unsigned long delay, ro::CDir direction, float z) {

	m_delay += delay;
	const float d = act->getDelay(m_action * 8 + direction) * 25;
	while (m_delay > d) {
		m_delay -= d;
		m_frame++;
	}
	const ro::ACT::Action& a = act->getAction(m_action * 8 + direction);
/*	if (loop) {
		if(a.getMotionCount() > 0 && m_frame >= a.getMotionCount()) {
			m_frame = m_frame % a.getMotionCount();
		}
	}
	else {
		m_frame = a.getMotionCount() - 1;
	}*/
	if(a.getMotionCount() > 0 && m_frame >= a.getMotionCount()) {
		if (m_loop) {
			m_frame = m_frame % a.getMotionCount();
		}
		else {
			m_frame = a.getMotionCount() - 1;
			m_playing = false;
		}
	}
	const ro::ACT::Motion& p = a.getMotion(m_frame);

	glPushMatrix();

	// Cylindrical Billboard
	// Credits: http://www.lighthouse3d.com/opengl/billboarding/index.php3?billCheat1
	float modelview[16];
	int i,j;
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	for (i = 0; i < 3; i += 2)
		for (j = 0; j < 3; j++)
			if (i == j)
				modelview[i * 4 + j] = 1.0f;
			else
				modelview[i * 4 + j] = 0.0f;
	glLoadMatrixf(modelview);
	// End billboard


	spr.getTexture().Activate();
	float u[2], v[2];
	//float aux;
	int sprcount = p.getClipCount();
	int w, h;

	for (int i = 0; i < sprcount; i++) {
		const ro::ACT::SprClip& s = p.getClip(i);
		if (s.sprNo < 0) {
			continue;
		}
		w = s.w;
		h = s.h;

		const rogl::sprInfo& info = spr.getFrameInfo(spr.getIndex(s.sprNo, s.sprType));
		if (w == 0)
			w = info.w;
		if (h == 0)
			h = info.h;

		// Setup coordinates
	//	v[0] = info.sv;
	//	v[1] = info.ev;
		v[1] = info.sv;
		v[0] = info.ev;
		if (!s.mirrorOn) {
			u[0] = info.su;
			u[1] = info.eu;
		}
		else {
			u[0] = info.eu;
			u[1] = info.su;
		}

		float x[2];
		float y[2];

		float xx = s.x;
		float yy = s.y;

		if (ext != NULL) {
			if (ext->getAct() != NULL) {
			//	const ro::ACT::Action& pact = ext->getAct()->getAction(act_no);
				const ro::ACT::Motion& pmot = ext->getAct()->getMotion(m_action * 8 + direction, m_frame);
				if (pmot.attachPoints.size() > 0) {
					xx += pmot.attachPoints[0].x;
					yy += pmot.attachPoints[0].y;
				}
			//	const ro::ACT::Motion& mot = a.getMotion(m_action, m_frame);
				if (p.attachPoints.size() > 0) {
					xx -= p.attachPoints[0].x;
					yy -= p.attachPoints[0].y;
				}
			}
		}
		x[0] =  (float)xx - ceil((float)w / 2.0f);
		x[1] =  x[0] + w;

		y[0] =  -((float)yy - ceil((float)h / 2.0f));
		y[1] =  y[0] - h;

		// Set things straight
		x[0] /= 7.0f;// + (float)s.x + 2.5f; // 2.5f = half of a gat tile.
		x[1] /= 7.0f;// + (float)s.x + 2.5f;
		y[0] /= 7.0f;
		y[1] /= 7.0f;

		float color[4];
		glGetFloatv(GL_CURRENT_COLOR, color);

		glColor4ubv((GLubyte*)&s.color);

		// Draw
		glBegin(GL_QUADS);
//		glBegin(GL_LINE_LOOP);
		glTexCoord2f(u[0], v[1]); glVertex3f(x[0], y[0], z + 3.5f);
		glTexCoord2f(u[0], v[0]); glVertex3f(x[0], y[1], z + 3.5f);
		glTexCoord2f(u[1], v[0]); glVertex3f(x[1], y[1], z + 3.5f);
		glTexCoord2f(u[1], v[1]); glVertex3f(x[1], y[0], z + 3.5f);
		glEnd();

		z += 0.01f;

		glColor4fv(color);
	}

	//spr.Draw(m_frame, true);
/*	if (ext != NULL) {
		if (p.attachPoints.size() > 0)
			glTranslatef((float)p.attachPoints[0].x / 10.0f, (float)p.attachPoints[0].y / 10.0f, 0.0f);
		ext->Draw(delay);
	}*/
#if 0
	glColor3f(1,0,0);
	Cross(20);
	glColor3f(1,1,1);
#endif
	glPopMatrix();
}

ActGL& ActGL::operator = (const ActGL& _act) {
	this->act = _act.act;
	this->spr = _act.spr;
	this->ext = _act.ext;
	return(*this);
}

void ActGL::setExt(ActGL* e) {
	ext = e;
}

void ActGL::setAct(ro::ACT* a) {
	act = a;
}

void ActGL::setSpr(const SprGL& s) {
	spr = s;
}

ActGL* ActGL::getExt() {
	return(ext);
}

ro::ACT* ActGL::getAct() {
	return(act);
}

SprGL& ActGL::getSpr() {
	return(spr);
}


void ActGL::Cross(float size) {
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glVertex3f(- size, 0,0);
	glVertex3f(+ size, 0,0);
	glVertex3f(0, - size,0);
	glVertex3f(0, + size,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}


}
