/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/button.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

#include <SDL/SDL.h>

GUI::Button::Button(Element* parent, const rogl::Texture::Pointer& base) : Element(parent) {
	texture_base = base;
	texture_active = base;
	texture_hover = base;
	texture_disabled = base;
}
GUI::Button::Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = active;
	texture_disabled = base;
}

GUI::Button::Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active, const rogl::Texture::Pointer& hover) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = hover;
	texture_disabled = base;
}

GUI::Button::Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active, const rogl::Texture::Pointer& hover, const rogl::Texture::Pointer& disabled) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = hover;
	texture_disabled = disabled;
}

GUI::Button::Button(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) : Element(parent) {
	if (node != NULL)
		ParseFromXml(node, tm, fm);

	texture_base = texture;

	if (!texture_active.isValid())
		texture_active = texture;
	if (!texture_hover.isValid())
		texture_hover = texture;
	if (!texture_disabled.isValid())
		texture_disabled = texture;
}

bool GUI::Button::ParseXmlAttr(const TiXmlAttribute* attr, TextureManager& tm, FileManager& fm) {
	if (GUI::Element::ParseXmlAttr(attr, tm, fm))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "active") {
		std::string tn = attr->Value();
		if (tn[0] != '\\') {
			std::string aux = "texture\\";
			aux += RO::EUC::user_interface;
			aux += "\\";
			tn = aux + tn;
		}
		else {
			tn = tn.substr(1);
		}

		texture_active = tm.Register(fm, tn);
		return(true);
	}
	else if (attrname == "hover") {
		std::string tn = attr->Value();
		if (tn[0] != '\\') {
			std::string aux = "texture\\";
			aux += RO::EUC::user_interface;
			aux += "\\";
			tn = aux + tn;
		}
		else {
			tn = tn.substr(1);
		}

		texture_hover = tm.Register(fm, tn);
		return(true);
	}
	else if (attrname == "disabled") {
		std::string tn = attr->Value();
		if (tn[0] != '\\') {
			std::string aux = "texture\\";
			aux += RO::EUC::user_interface;
			aux += "\\";
			tn = aux + tn;
		}
		else {
			tn = tn.substr(1);
		}

		texture_disabled = tm.Register(fm, tn);
		return(true);
	}

	return(false);
}

void GUI::Button::Click() {
	Event e(getName(), Event::evtClick, this);
	GUI::Gui::getSingleton().PushEvent(e);
}


bool GUI::Button::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);
	Click();
	return(true);
}

bool GUI::Button::HandleKeyDown(int key, int mod) {
	if (!m_enabled)
		return(false);

	if (key == SDLK_SPACE || key == SDLK_RETURN) {
		Click();
		return(true);
	}
	return(GUI::Element::HandleKeyDown(key, mod));
}

void GUI::Button::Draw(unsigned int delay) {
	// Default
	texture = texture_base;

	// Are we the active button?
	if (m_parent != NULL) {
		if (m_parent->getActiveChild() == this) {
			texture = texture_active;
		}
	}

	Element::Draw();
}
