#include "stdafx.h"
#include "sdle/console.h"

#ifdef SDLENGINE_CONSOLE

#include "sdle/sdl_engine.h"
#include "sdle/glf_font.h"

namespace sdle {

Console::Console() {
	m_activate_key = SDLK_BACKQUOTE;
	m_activate_mod = KMOD_LALT;
	m_active = false;
	m_activated = false;
	m_linecount = 8;
	m_font = (Font*)Font_Arial10;
	add("Console initialized...");
}

Console::~Console() {
}

void Console::add(const std::string& str) {
	m_lines.insert(m_lines.begin(), str);
}

bool Console::setFont(Font* f) {
	m_font = f;
	if (m_font == NULL)
		m_active = false;

	return(true);
}

void Console::setVisible(bool v) {
	m_active = v;
	if (m_font == NULL)
		m_font = (Font*)Font_Arial10;

	if (m_font == NULL)
		m_active = false;
}

bool Console::InjectKeyDown(const SDL_Event* e) {
	if (e->key.keysym.sym == m_activate_key) {
		setVisible(!m_active);
		return(true);
	}

	if (m_active == false)
		return(false);

	m_commandline += (char)e->key.keysym.sym;

	return(true);
}

bool Console::InjectKeyUp(const SDL_Event* e) {
	if (m_active == false)
		return(false);

	return(true);
}

void Console::Draw() {
	if (!m_active)
		return;

	SDLEngine* engine;
	engine = SDLEngine::getSingleton();
	if (engine == NULL)
		return;

	int font_w, font_h;
	unsigned short s[2] = { 32, 0 };

	m_font->getSize(s, &font_w, &font_h);

	int console_h = font_h * (m_linecount + 1);

	engine->Mode2DStart();

	glColor3f(0.4f, 0, 0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glVertex3i(0, 0, 0);
	glVertex3i(engine->getWidth(), 0, 0);
	glVertex3i(engine->getWidth(), console_h, 0);
	glVertex3i(0, console_h, 0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int h_pos = font_h * m_linecount;
	m_font->drawText(10, h_pos, "> %s", m_commandline.c_str());
	
	std::vector<std::string>::iterator itr = m_lines.begin();
	for (int i = 0; i < m_linecount; i++) {
		if (itr == m_lines.end())
			break;
		h_pos -= font_h;
		m_font->drawText(10, h_pos, (*itr).c_str());
		itr++;
	}

	engine->Mode2DEnd();
}



}

#endif /* SDLENGINE_CONSOLE */
