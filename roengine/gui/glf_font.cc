/* $Id$ */
#include "stdafx.h"

#include "glf_font.h"
#include "../blob.h"

#include <iostream>
#include <fstream>

GUI::GLFFont::GLFFont() {
	m_font.Char = NULL;
}

GUI::GLFFont::GLFFont(const std::string & fn) {
	m_font.Char = NULL;
	load(fn);
}

GUI::GLFFont::~GLFFont() {
	// clear chars
	// clear texture
}

bool GUI::GLFFont::load(const std::string & fn) {
	int num;

	std::ifstream file(fn.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!file.is_open()) {
		return(false);
	}
	file.read((char*)&m_font, sizeof(Font));
	glGenTextures(1, &m_font.Tex);
	
	num = m_font.IntEnd - m_font.IntStart + 1; // Number of characters
	m_font.Char = new Char[num];
	file.read((char*)m_font.Char, sizeof(Char) * num);

	num = m_font.TexHeight * m_font.TexWidth * 2; // Texture size
	DynamicBlob<unsigned char> TexBytes;
	TexBytes.setSize(num);
	file.read((char*)TexBytes.getBuffer(), num);

	glBindTexture(GL_TEXTURE_2D, m_font.Tex);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
	
	//Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, 2, m_font.TexWidth,
		m_font.TexHeight, 0, GL_LUMINANCE_ALPHA, 
		GL_UNSIGNED_BYTE, (void *)TexBytes.getBuffer());

	file.close();
	return(true);
}

void GUI::GLFFont::textOut(const std::string& data, float x, float y, float z) const {
	float fontmult = 6.0f;

	glBindTexture(GL_TEXTURE_2D, m_font.Tex);

	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < data.length(); i++) {
			const Char& c = m_font.Char[data[i] - m_font.IntStart];

		glTexCoord2f(c.tx1, c.ty1);
		glVertex3f(x, y, z);
		glTexCoord2f(c.tx1, c.ty2);
		glVertex3f(x, y + c.dy * fontmult, z);
		glTexCoord2f(c.tx2, c.ty2);
		glVertex3f(x + c.dx * fontmult, y + c.dy * fontmult, z);
		glTexCoord2f(c.tx2, c.ty1);
		glVertex3f(x + c.dx * fontmult, y, z);

		//Move to next character
		x += c.dx * fontmult;
	}
	glEnd();
}
