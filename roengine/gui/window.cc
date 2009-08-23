/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/window.h"
#include "roengine/gui/gui.h"

GUI::Window::Window() : Element() {
	IsMouseDowning = false;
}

GUI::Window::Window(Element* parent) : Element(parent) {
	IsMouseDowning = false;
}

GUI::Window::Window(const rogl::Texture::Pointer& t, Element* parent) : Element(parent) {
	IsMouseDowning = false;
	texture = t;
}

GUI::Window::Window(Element* parent, class TiXmlElement const * xml, TextureManager& tm, FileManager& fm) : Element(parent, xml, tm, fm) {
	IsMouseDowning = false;
}

GUI::Window::Window(Element* parent, const std::string& background, TextureManager& tm, FileManager& fm) : Element(parent, background, tm, fm) {
	IsMouseDowning = false;
}

GUI::Window::~Window() {}

void GUI::Window::setCaption(const std::string& n) {
	caption = n;
}

bool GUI::Window::HandleMouseRelease(int x, int y, int button) {
	std::cout << getName() << "::MouseRelease (" << x << ", " << y << ")" << std::endl;

	std::vector<Element*>::iterator itr = m_children.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if( this == gui.getDesktop() )
	{
		if(itr != m_children.end()) {
			Element* e = *itr;

			if ( button == 1 && isInside(e, x, y) && e->isVisible() && isInsideMoveArea(e, x, y) ) {
				std::cout << getName() << "::MouseDownOnWindowMoveArea (" << x << ", " << y << ")" << std::endl;
				IsMouseDowning = false;
				e->setStransparent(false);
			}
		}
	}

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y)) {
			if (e->isVisible())
				return(e->HandleMouseRelease(x - e->getX(), y - e->getY(), button));
		}
		itr++;
	}

	return(false);
}

bool GUI::Window::HandleMouseDown(int x, int y, int button) {

	if (!m_enabled)
		return(false);

	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;
	std::vector<Element*>::iterator itr = m_children.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if( this == gui.getDesktop() )
	{
		if(itr != m_children.end()) {
			Element* e = *itr;

			if ( button == 1 && isInside(e, x, y) && e->isVisible() && isInsideMoveArea(e, x, y) ) {
				std::cout << getName() << "::MouseDownOnWindowMoveArea (" << x << ", " << y << ")" << std::endl;
				IsMouseDowning = true;
				e->setStransparent(true);
			}
		}
	}

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y)) {
			if (e->isVisible())
				return(e->HandleMouseDown(x - e->getX(), y - e->getY(), button));
		}
		itr++;
	}
	return(true);
}

bool GUI::Window::HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy) {

	if (!m_enabled)
		return(false);

	std::vector<Element*>::iterator itr = m_children.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if( this == gui.getDesktop() )
	{
		if(itr != m_children.end()) {
			Element* e = *itr;

			if ( IsMouseDowning ) {
				//std::cout << getName() << "::MouseMove (" << x << ", " << y << ")" << std::endl;
				e->setPos(e->getX()+dx,e->getY()+dy);
			}
		}
	}

	//判断是否鼠标在控件上
	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y)) {
			//std::cout << e->getName() << "::MouseIn (" << x << ", " << y << ")" << std::endl;
			e->SetMouseInFlag(true);
			e->HandleMouseMove(x - e->getX(), y - e->getY(), dx, dy);
		}
		else
		{
			//std::cout << e->getName() << "::MouseOut (" << x << ", " << y << ")" << std::endl;
			e->SetMouseInFlag(false);
			e->HandleMouseMove(x - e->getX(), y - e->getY(), dx, dy);
		}
		itr++;
	}
	return(true);
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
