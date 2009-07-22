/* $Id$ */
#include "stdafx.h"

#include "roengine/objects/cube_object.h"

#include <GL/gl.h>
#include <GL/glu.h>

CubeObject::CubeObject(const float& size) : GLObject() {
	m_size = size;
}

void CubeObject::Draw() {
	float aux = m_size / 2;

	float u = 1.0f;
	float v = 1.0f;
	if (m_texture.isValid()) {
		u = m_texture->getMaxU();
		v = m_texture->getMaxV();
	}
	// Thanks, NeHe! http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0, 0); glVertex3f(-aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(u, 0); glVertex3f( aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(u, v); glVertex3f( aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0, v); glVertex3f(-aux,  aux,  aux);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(u, 0); glVertex3f(-aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(u, v); glVertex3f(-aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0, v); glVertex3f( aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0, 0); glVertex3f( aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0, v); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0, 0); glVertex3f(-aux,  aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(u, 0); glVertex3f( aux,  aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(u, v); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(u, v); glVertex3f(-aux, -aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0, v); glVertex3f( aux, -aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0, 0); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(u, 0); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(u, 0); glVertex3f( aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(u, v); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0, v); glVertex3f( aux,  aux,  aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0, 0); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0, 0); glVertex3f(-aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(u, 0); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(u, v); glVertex3f(-aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0, v); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
	glEnd();
}
