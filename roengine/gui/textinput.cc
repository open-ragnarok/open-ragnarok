/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/textinput.h"
#include "roengine/gui/gui.h"
#include "roengine/gui/font.h"

#include <GL/gl.h>
#include <GL/glu.h>

unsigned int GUI::TextInput::cursor_delay = 160;

GUI::TextInput::TextInput(Element* parent, const TiXmlElement* e, TextureManager& tm, FileManager& fm) : Label(parent, e, tm, fm) {
	m_focusable = true;
	m_delay = 0;
	m_pos = m_text.size();
	m_start = 0;
	m_bar = true;
}

void GUI::TextInput::Draw(unsigned int delay) {
	m_delay += delay;
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const GUI::Font* font = gui.getDefaultFont();

	GUI::Label::Draw(delay);
	int x = (int)(font->getWidth(m_text.substr(m_start, m_pos - m_start)));

	while (m_delay > cursor_delay) {
		m_delay -= cursor_delay;
		m_bar = !m_bar;
	}
	if (m_bar) {
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex3i(x + pos_x, pos_y, 0);
		glVertex3i(x + pos_x, pos_y + 16, 0);
		glEnd();
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
	}
}

bool GUI::TextInput::HandleKeyDown(int key, int mod) {
	// Do not handle key input with CTRL, ALT or META.
	int modflag = KMOD_LCTRL | KMOD_RCTRL | KMOD_LALT | KMOD_RALT | KMOD_LMETA | KMOD_RMETA;
	if (mod & modflag) {
		if (m_parent == NULL)
			return(false);
		return(m_parent->HandleKeyDown(key, mod));
	}

	if (key == SDLK_BACKSPACE) {
		if (m_pos > m_text.length())
			m_pos = m_text.length();
		if (m_pos <= 0)
			return(true);
		m_text.erase(m_pos - 1, 1);
		m_pos--;
		return(true);
	}

	if (key == SDLK_HOME) {
		m_pos = 0;
		return(true);
	}

	if (key == SDLK_END) {
		m_pos = m_text.length();
		return(true);
	}

	if (key == SDLK_DELETE) {
		if (m_pos == m_text.length())
			return(true);
		m_text.erase(m_pos, 1);
		return(true);
	}

	if (key == SDLK_LEFT) {
		if(m_pos == 0)
			return(true);
		m_pos--;
	}

	if (key == SDLK_RIGHT) {
		m_pos++;
		if (m_pos > m_text.size())
			m_pos = m_text.size();
	}

	if ((key >= SDLK_a && key <= SDLK_z) || (key == SDLK_SPACE)) {
		char buf[2];
		if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
			sprintf(buf, "%c", key + 'A' - 'a');
		else
			sprintf(buf, "%c", key);

		if (m_pos >= m_text.length()) {
			m_text += buf;
			m_pos++;
		}
		else {
			sprintf(buf, "%c", key);
			m_text.insert(m_pos++, buf);
		}
		return(true);
	}

	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(key, mod));
}
