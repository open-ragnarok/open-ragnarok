/* $Id$ */
#include "stdafx.h"

#include "roengine/gl_object.h"

#include <GL/gl.h>
#include <GL/glu.h>

GLObject::GLObject() {
	xrot=yrot=zrot=0.0f;
	scalex=scaley=scalez=1.0f;
	m_useTexture = false; 
	m_visible = true;
	m_frustum_check = true;
	cameraDir = ro::DIR_N;
}

GLObject::~GLObject() {}

void GLObject::setVisible(bool v) {
	m_visible = v;
}

void GLObject::AfterDraw() {
	if (m_useTexture)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

bool GLObject::isVisible() const {
	return(m_visible);
}

void GLObject::BeforeDraw() {
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(-xrot, 1, 0, 0);
	glRotatef(-zrot, 0, 0, 1);
	glRotatef(yrot, 0, 1, 0);
	glScalef(scalex, scaley, scalez);
	if (m_useTexture) {
		glEnable(GL_TEXTURE_2D);
		m_texture.Activate();
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}
}

void GLObject::Render(long tickdelay, const Frustum* f, ro::CDir CameraLook) {
	m_tickdelay = tickdelay;
	cameraDir = CameraLook;
	m_frustum = f;
	BeforeDraw();
	Draw();
	AfterDraw();
}

void GLObject::setPos(const float& x, const float& y, const float& z) {
	pos.set(x, y, z);
}

void GLObject::setRot(const float& x, const float& y, const float& z) {
	this->xrot = x;
	this->yrot = y;
	this->zrot = z;
}

void GLObject::setScale(const float& x, const float& y, const float& z) {
	scalex = x;
	scaley = y;
	scalez = z;
}

Vector3f& GLObject::getPos() {
	return(pos);
}

const Vector3f& GLObject::getPos() const {
	return(pos);
}

void GLObject::setRotX(const float& x) {
	xrot = x;
}

void GLObject::setRotY(const float& y) {
	yrot = y;
}

void GLObject::setRotZ(const float& z) {
	zrot = z;
}

void GLObject::setTexture(sdle::Texture& tex) {
	m_texture = tex;
}

void GLObject::useTexture(bool b) {
	m_useTexture = b;
}

void GLObject::setFrustumCheck(bool b) {
	m_frustum_check = b;
}

bool GLObject::isInFrustum(const Frustum* f) const {
	if (!m_frustum_check)
		return(true);
	return(f->PointVisible(pos[0], pos[1], pos[2]));
}
