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

	spr.Draw(m_frame);
	Cross(10);
}

ActGL& ActGL::operator = (const ActGL& _act) {
	this->act = _act.act;
	this->spr = _act.spr;
	this->ext = _act.ext;
	return(*this);
}

void ActGL::setExt(const ActGL* e) {
	ext = e;
}

void ActGL::setAct(RO::ACT* a) {
	act = a;
}

void ActGL::setSpr(const SprGL& s) {
	spr = s;
}

const ActGL* ActGL::getExt() {
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