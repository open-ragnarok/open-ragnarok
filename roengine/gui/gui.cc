/* $Id$ */
#include "stdafx.h"

#include "gui.h"

namespace GUI {
#include "arabia-8.glf.h"
#include "arial-8.glf.h"
#include "arial-10.glf.h"


void Mode2DStart(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

void Mode2DEnd() {
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

Font* getDefaultFont(const unsigned char data[], unsigned int data_size) {
	std::stringstream ss;
	GLFFont* f = new GLFFont();
	ss.write((char*)data, data_size);
	if (!f->load(ss)) {
		delete(f);
		return(NULL);
	}
	return((Font*)f);
}

void LoadDefaultFonts() {
	Gui& gui = Gui::getSingleton();
	Font* f;

	f = getDefaultFont(glf_arial8, glf_arial8_size);
	gui.FontManager().add("arial-8", f);

	f = getDefaultFont(glf_arial10, glf_arial10_size);
	gui.FontManager().add("arial-10", f);

	f = getDefaultFont(glf_arabia8, glf_arabia8_size);
	gui.FontManager().add("arabia-8", f);
}
}

void GUI::Gui::Init(int w, int h) {
	setSize(w, h);

	LoadDefaultFonts();

	m_defaultFont = m_fonts["arial-8"];
}

GUI::Gui::Gui() : Singleton() {
	m_desktop = NULL;
	active = NULL;
	m_defaultFont = NULL;
}

GUI::Gui::~Gui() {
}

void GUI::Gui::Draw() {
	if (!m_desktop)
		return;
	Mode2DStart(m_width, m_height);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.90);

	m_desktop->Draw();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	Mode2DEnd();
}

void GUI::Gui::setSize(int w, int h) {
	m_width = w;
	m_height = h;
}

GUI::Element* GUI::Gui::getDesktop() {
	return(m_desktop);
}

void GUI::Gui::setDesktop(Element* e) {
	m_desktop = e;
}

BaseCache<GUI::Font>& GUI::Gui::FontManager() {
	return(m_fonts);
}

const BaseCache<GUI::Font>& GUI::Gui::FontManager() const {
	return(m_fonts);
}

const GUI::Font* GUI::Gui::getDefaultFont() {
	return(m_defaultFont);
}

int GUI::Gui::getWidth() const {
	return(m_width);
}

int GUI::Gui::getHeight() const {
	return(m_height);
}
