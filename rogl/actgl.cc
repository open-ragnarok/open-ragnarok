/* $Id$ */
#include "stdafx.h"

#include "rogl/actgl.h"

namespace rogl {

ActGL::ActGL() {
	act = NULL;
	ext = NULL;
	m_action = 0;
	m_delay = 0;
	m_frame = 0;
}

ActGL::ActGL(const ActGL& _act) {
	*this = _act;
}

ActGL::~ActGL() {
}

bool ActGL::valid() const {
	return(act != NULL);
}


void ActGL::Draw() const {
}


void ActGL::Draw(unsigned long delay) {
	m_delay += delay;
	while (m_delay > 200) {
		m_delay -= 200;
		m_frame += 1;
		if (m_frame >= spr.getFrameCount())
			m_frame = 0;
	}

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

	spr.Draw(m_frame, true);
	if (ext != NULL) {
		ext->Draw(delay);
	}
	glColor3f(1,0,0);
	Cross(10);
	glColor3f(1,1,1);
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

void ActGL::setAct(RO::ACT* a) {
	act = a;
}

void ActGL::setSpr(const SprGL& s) {
	spr = s;
}

ActGL* ActGL::getExt() {
	return(ext);
}

RO::ACT* ActGL::getAct() {
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