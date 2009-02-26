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
	clear();
}

void GUI::Gui::clear() {
	GUI::Element::getCache().clear();
}

void GUI::Gui::Draw(unsigned int delay, Vector3f CameraLook) {
	if (!m_desktop)
		return;
	Mode2DStart(m_width, m_height);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.45f);

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

const GUI::Element* GUI::Gui::getDesktop() const {
	return(m_desktop);
}

void GUI::Gui::setDesktop(Element* e) {
	m_desktop = e;
}

void GUI::Gui::setDesktop(const std::string& ui) {
	Element* e = GUI::Element::getElement(ui);
	if (e == NULL) {
		std::cerr << "No GUI element named " << ui << " found." << std::endl;
		return;
	}
	setDesktop(e);
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

GUI::Element* GUI::Gui::getActiveElement() {
	if (active == NULL) {
		if (m_desktop != NULL) {
			Element* next;
			active = m_desktop;
			next = active->getActiveChild();
			while (next != NULL) {
				active = next;
				next = next->getActiveChild();
			}
		}
	}
	return(active);
}

bool GUI::Gui::InjectKeyPress(const int& key, const int& mod) {
	Element* e = getActiveElement();
	if (e == NULL)
		return(false);
	return(e->HandleKeyDown(key, mod));
}

bool GUI::Gui::InjectKeyRelease(const int& key, const int& mod) {
	Element* e = getActiveElement();
	if (e == NULL)
		return(false);
	return(e->HandleKeyUp(key, mod));
}

bool GUI::Gui::InjectMouseClick(int x, int y, int buttons, int modifier)  {
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseDown(x, y, buttons));
}

void GUI::Gui::setFocus(Element* e) {
	if (active != e) {
		if (active != NULL)
			active->onLoseFocus();
		active = e;
		active->setActive();
		if (active != NULL)
			active->onGetFocus();
	}
}

GUI::Element* GUI::Gui::operator[] (const std::string& n) {
	return(Element::getElement(n));
}

const GUI::Element* GUI::Gui::operator[] (const std::string& n) const {
	return(Element::getElement(n));
}

void GUI::Gui::PushEvent(const Event& e) {
	m_events.push_back(e);
}
	
GUI::Event GUI::Gui::PopEvent() {
	std::vector<Event>::iterator itr;
	itr = m_events.begin();
	if (itr == m_events.end())
		return(Event());
	Event ret = *itr;
	m_events.erase(itr);
	return(ret);
}

int GUI::Gui::GetEventCount() const {
	return(m_events.size());
}

