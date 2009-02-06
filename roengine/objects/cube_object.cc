/* $Id$ */
#include "stdafx.h"

#include "cube_object.h"

CubeObject::CubeObject(const float& size) : GLObject() {
	m_size = size;
}

void CubeObject::Draw() {
	float aux = m_size / 2;
	// Thanks, NeHe! http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux,  aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux,  aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux, -aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux, -aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
	glEnd();
}