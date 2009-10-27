/*
 * frame.cc
 *
 *  Created on: 14/07/2009
 *      Author: N0C2
 */
#include "stdafx.h"
#include "sdle/frame.h"
#include "sdle/sdl_engine.h"

namespace sdle {
namespace gui {
Frame::Frame() {
	m_visible = true;
}

Frame::~Frame() {
}

void Frame::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

int Frame::getY() const {
	return(y);
}

void Frame::Draw() {
	glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
	glBegin(GL_QUADS);
	glVertex3i(x, y, 0);
	glVertex3i(x, y+h, 0);
	glVertex3i(x+w, y+h, 0);
	glVertex3i(x+w, y, 0);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3i(x, y, 0);
	glVertex3i(x, y+h, 0);
	glVertex3i(x+w, y+h, 0);
	glVertex3i(x+w, y, 0);
	glEnd();
}

int Frame::getHeight() const {
	return(h);
}

int Frame::getX() const {
	return(y);
}

void Frame::setDimension(int w, int h) {
	this->w = w;
	this->h = h;
}

bool Frame::isVisible() const {
	return(m_visible);
}

int Frame::getWidth() const {
	return(w);
}

void Frame::setVisible(bool v) {
	m_visible = v;
}

}
}

