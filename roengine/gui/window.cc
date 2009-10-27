/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/window.h"
#include "roengine/gui/gui.h"

GUI::Window::Window() : Element() {
}

GUI::Window::Window(Element* parent) : Element(parent) {
}

GUI::Window::Window(const sdle::Texture& t, Element* parent) : Element(parent) {
	texture = t;
}

GUI::Window::Window(Element* parent, const TiXmlElement* xml, CacheManager& cache) : Element(parent, xml, cache) {
}

GUI::Window::Window(Element* parent, const std::string& background, CacheManager& cache) : Element(parent, background, cache) {
}

GUI::Window::~Window() {}

void GUI::Window::setCaption(const std::string& n) {
	caption = n;
}

bool GUI::Window::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.sym;
	if (key != SDLK_TAB) {
		if (m_parent != NULL)
			return(m_parent->HandleKeyDown(sdlEvent, mod));
		return(false);
	}

	std::vector<Element*>::iterator itr = m_children.begin();

	if (m_active_child == NULL) {
		while (itr != m_children.end()) {
			if ((*itr)->isFocusable())  {
				m_active_child = *itr;
				GUI::Gui& gui = GUI::Gui::getSingleton();
				gui.setFocus(m_active_child);
				return(true);
			}
			itr++;
		}
		return(true);
	}

	while (itr != m_children.end()) {
		if (*itr == m_active_child) {
			itr++;
			if (itr == m_children.end())
				itr = m_children.begin();

			while (*itr != m_active_child) {
				if (itr == m_children.end())
					itr = m_children.begin();
				if ((*itr)->isFocusable()) {
					m_active_child = *itr;
					GUI::Gui& gui = GUI::Gui::getSingleton();
					gui.setFocus(m_active_child);
					return(true);
				}
				itr++;
			}
			return(true);
		}
		itr++;
	}
	return(true);
}
