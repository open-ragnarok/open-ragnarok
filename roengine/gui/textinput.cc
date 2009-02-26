/* $Id$ */
#include "stdafx.h"

#include "textinput.h"
#include "gui.h"
#include "font.h"

#include "sdl.h"

GUI::TextInput::TextInput(Element* parent, const TiXmlElement* e, TextureManager& tm, FileManager& fm) : Label(parent, e, tm, fm) {
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

	if (key == SDLK_DELETE) {
		if (m_pos >= m_text.length() - 1)
			return(true);
		m_text.erase(m_pos, 1);
		return(true);
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
