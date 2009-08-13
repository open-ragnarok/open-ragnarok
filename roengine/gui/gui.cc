/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/gui.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "roengine/gui/glf_font.h"

namespace GUI {
#include "roengine/gui/arabia-8.glf.h"
#include "roengine/gui/arial-8.glf.h"
#include "roengine/gui/arial-10.glf.h"

/**
 * Starts OpenGL 2D Mode
 * @param width int the window width
 * @param height int the window height
 */
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

/**
 * Ends OpenGL 2D Mode and resets the matrix back to the state
 * when Mode2DStart() was called.
 */
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

/**
 * Loads the basic fonts for the engine (even if there are no external fonts available,
 * these will be).
 * Provided fonts are Arial (sizes 8 and 10) and Arabia (size 8).
 */
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

	m_defaultFont = m_fonts["arial-10"];
}

GUI::Gui::Gui() : Singleton<Gui>() {
	m_desktop = NULL;
	m_inactiveDesktop = NULL;
	active = NULL;
	m_defaultFont = NULL;
	m_msgbox_bg = "login_interface\\win_service.bmp";
	m_msgbox_btnok = "login_interface\\btn_ok.bmp";
	m_msgbox_btncancel = "login_interface\\btn_cancel.bmp";
	m_msgbox_btnexit = "login_interface\\btn_exit.bmp";
}

GUI::Gui::~Gui() {
	clear();
}

const std::string GUI::Gui::getMsgboxBg() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_bg;
	return(aux);
}

const std::string GUI::Gui::getMsgboxOk() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btnok;
	return(aux);
}

const std::string GUI::Gui::getMsgboxCancel() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btncancel;
	return(aux);
}

const std::string GUI::Gui::getMsgboxExit() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btnexit;
	return(aux);
}

void GUI::Gui::clear() {
	GUI::Element::getCache().clear();
}

void GUI::Gui::Draw(unsigned int delay, Vector3f CameraLook) {
	if (!m_desktop && !m_inactiveDesktop)
		return;

	Mode2DStart(m_width, m_height);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.45f);

	if (m_inactiveDesktop) {
		if (!m_desktop->isVisible()) {
			delete(m_desktop);
			m_desktop = m_inactiveDesktop;
			m_inactiveDesktop = NULL;
		}
		else {
			m_inactiveDesktop->Draw(delay);
		}
	}

	if (m_desktop)
		m_desktop->Draw(delay);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	Mode2DEnd();
}

void GUI::Gui::textOut(const std::string& text, float x, float y, float z,int MaxLen) {
	m_defaultFont->textOut(text, x, y, z,MaxLen);
}

void GUI::Gui::setSize(int w, int h) {
	m_width = w;
	m_height = h;
}

GUI::Desktop* GUI::Gui::getDesktop() {
	return(m_desktop);
}

const GUI::Desktop* GUI::Gui::getDesktop() const {
	return(m_desktop);
}

void GUI::Gui::setDesktop(Desktop* e) {
	if (m_desktop == e)
		return;
	m_desktop = e;
	if (m_desktop == NULL)
		return;

	active = (Element*)m_desktop;
	while (active->getActiveChild() != NULL) {
		active = active->getActiveChild();
	}
}

bool GUI::Gui::setDesktop(const std::string& ui) {
	if (ui == "") {
		setDesktop(NULL);
		return(true);
	}
	Element* e = GUI::Element::getElement(ui);
	if (e == NULL) {
		std::cerr << "No GUI element named " << ui << " found." << std::endl;
		return(false);
	}
	setDesktop((Desktop*)e);
	return(true);
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

bool GUI::Gui::InjectMouseRelease(int x, int y, int buttons, int modifier)  {
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseRelease(x, y, buttons));
}

bool GUI::Gui::InjectMouseMove(const int& x, const int& y, const int& dx, const int& dy)
{
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseMove(x, y, dx, dy));
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

void GUI::Gui::ProcessEvents() {
	if (m_desktop == NULL)
		return;

	while (GetEventCount())
		m_desktop->HandleEvent(PopEvent());
}

void GUI::Gui::Dialog(const std::string& title, const std::string& text, TextureManager& tm, FileManager& fm, int buttons) {
	GUI::Dialog* dialog;
	dialog = new GUI::Dialog(title, text, tm, fm);
	m_inactiveDesktop = m_desktop;
	m_desktop = dialog;
	dialog->setVisible(true);
}
