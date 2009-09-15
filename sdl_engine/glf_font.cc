/* $Id$ */
#include "stdafx.h"

#include "sdle/glf_font.h"
#include "sdle/blob.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>

namespace sdle {
/* Note from Sergio:
 * I can't find any documentation on the GLF format nor remember where I got it from.
 * It's basicaly like this:
 * unsigned int <placeholder for texture id>
 * int <texture width> int <texture height>
 * int <character start> int <character end>
 * CHARACTER LIST (array of (character end - character start) items with info on how to draw each char
 *    float <x offset> float <y offset>
 *    float <start u> float <start v>
 *    float <end u> float <end v>
 * The data (2 bytes per pixel, resulting in a buffer size of (texture_width * texture_height * 2) long.
 */

GLFFont* Font_Arabia8;
GLFFont* Font_Arial8;
GLFFont* Font_Arial10;

GLFFont::GLFFont() : Font() {
	m_font.Char = NULL;
	fontmult = 6.0f;

	printf("[DEBUG] Creating GLF Font...\n");
}

GLFFont::GLFFont(const unsigned char* data, unsigned int size) : Font() {
	m_font.Char = NULL;
	fontmult = 6.0f;

	std::stringstream ss;
	ss.write((const char*)data, size);
	load(ss);
	printf("[DEBUG] Creating GLF Font...\n");
}

GLFFont::~GLFFont() {
	printf("[DEBUG] Destroying GLF Font...\n");
	release();
}

bool GLFFont::load(const std::string & fn) {
	bool ret;

	std::ifstream file(fn.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!file.is_open()) {
		return(false);
	}

	ret = load(file);

	file.close();
	m_valid = true;
	return(ret);
}

void GLFFont::release() {
	if (m_font.Char != NULL)
		delete[] m_font.Char;
	m_valid = false;
	m_texture = Texture();
}

bool GLFFont::load(std::istream& file) {
	if (valid())
		release();
	int num;
	file.read((char*)&m_font, sizeof(FontInfo));
	glGenTextures(1, &m_font.Tex);
	
	num = m_font.IntEnd - m_font.IntStart + 1; // Number of characters
	m_font.Char = new _Char[num];
	file.read((char*)m_font.Char, sizeof(_Char) * num);

	num = m_font.TexHeight * m_font.TexWidth * 2; // Texture size
	sdle::DynamicBlob TexBytes;
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

	Texture::Root* r = new Texture::Root();
	if (!r->Create(m_font.Tex, m_font.TexWidth, m_font.TexHeight)) {
		release();
		delete(r);
		return(false);
	}

	m_texture = r;
	m_valid = true;

	return(true);
}

void GLFFont::getSize(const unsigned short* text, int* w, int* h) const {
	unsigned int i;
	*h = 10; // TODO: Fix this.

	float rw = 0;

	for(i = 0; text[i] != 0; i++) {
		const _Char& c = m_font.Char[text[i] - m_font.IntStart];
		rw += c.dx * fontmult;
	}
	*w = (int)rw;
}

float GLFFont::getWidth(const std::string& s) const {
	float ret = 0;
	for (unsigned int i = 0; i < s.length(); i++) {
		const _Char& c = m_font.Char[s[i] - m_font.IntStart];
		ret += c.dx * fontmult;
	}

	return(ret);
}

void GLFFont::draw(const Rect& rect, const unsigned short* text) const {
	if (!valid()) return;

	float used_w = 0;

	m_texture.Activate();
	
	unsigned short ch;

	glPushMatrix();
	glTranslated(rect.x, rect.y, 0);
	glBegin(GL_QUADS);
	for (unsigned int i = 0; text[i] != 0; i++) {
		ch = text[i];
		if (ch > m_font.IntEnd)
			continue;

		const _Char& c = m_font.Char[ch - m_font.IntStart];

		if (rect.w > 0) {
			if ((used_w + c.dx) > rect.w)
				break;
		}

		glTexCoord2f(c.tx1, c.ty1); glVertex3f(0, 0, 0);
		glTexCoord2f(c.tx1, c.ty2); glVertex3f(0, c.dy * fontmult, 0);
		glTexCoord2f(c.tx2, c.ty2); glVertex3f(c.dx * fontmult, c.dy * fontmult, 0);
		glTexCoord2f(c.tx2, c.ty1); glVertex3f(c.dx * fontmult, 0, 0);
		glTranslatef(c.dx, 0.0f, 0.0f);

		used_w += c.dx;

	}
	glEnd();
	glPopMatrix();
}

void GLFFont::textOut(const std::string& data, float x, float y, float z, int MaxLen) const {
	if (!valid()) {
		// TODO: Throw exception?
		return;
	}

	if (MaxLen < 0)
		return;

	unsigned int i;
	m_texture.Activate();

	if(MaxLen > 0) {
		for(i = 0 ; i < data.length() ; i++) {
			if(getWidth(data.substr(i, data.length() - i)) <= MaxLen) {
				break;
			}
		}
	}
	else {
		i = 0;
	}

	glBegin(GL_QUADS);
	for (; i < data.length(); i++) {
		const _Char& c = m_font.Char[data[i] - m_font.IntStart];

		glTexCoord2f(c.tx1, c.ty1); glVertex3f(x, y, z);
		glTexCoord2f(c.tx1, c.ty2); glVertex3f(x, y + c.dy * fontmult, z);
		glTexCoord2f(c.tx2, c.ty2); glVertex3f(x + c.dx * fontmult, y + c.dy * fontmult, z);
		glTexCoord2f(c.tx2, c.ty1); glVertex3f(x + c.dx * fontmult, y, z);

		//Move to next character
		x += c.dx * fontmult;
	}
	glEnd();
}

}
