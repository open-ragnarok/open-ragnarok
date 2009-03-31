/* $Id$ */
#include "stdafx.h"

#include "window.h"
#include "sdl.h"
#include "gui.h"

GUI::Window::Window() : Element() {
}

GUI::Window::Window(const rogl::Texture::Pointer& t) : Element() {
	texture = t;
}

GUI::Window::~Window() {}

void GUI::Window::setCaption(const std::string& n) {
	caption = n;
}

GUI::Window::Window(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) : Element(parent, node, tm, fm) {

}

bool GUI::Window::HandleKeyDown(int key, int mod) {
	if (key != SDLK_TAB) {
		if (m_parent != NULL)
			return(m_parent->HandleKeyDown(key, mod));
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