/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/button.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

GUI::Button::Button(Element* parent) : Element(parent) {
	m_MouseIn = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base) : Element(parent) {
	texture_base = base;
	texture_active = base;
	texture_hover = base;
	texture_disabled = base;
	m_MouseIn = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = active;
	texture_disabled = base;
	m_MouseIn = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active, const sdle::Texture& hover) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = hover;
	texture_disabled = base;
	m_MouseIn = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active, const sdle::Texture& hover, const sdle::Texture& disabled) : Element(parent) {
	texture_base = base;
	texture_active = active;
	texture_hover = hover;
	texture_disabled = disabled;
	m_MouseIn = false;
}

GUI::Button::Button(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	if (node != NULL)
		ParseFromXml(node, cache);

	texture_base = texture;
	m_MouseIn = false;

	if (!texture_active.Valid())
		texture_active = texture;
	if (!texture_hover.Valid())
		texture_hover = texture;
	if (!texture_disabled.Valid())
		texture_disabled = texture;
}

GUI::Button::Button(Element* parent, const std::string& background, CacheManager& cache) : Element(parent, background, cache) {
	texture_base = texture;
	texture_active = texture;
	texture_hover = texture;
	texture_disabled = texture;
}

bool GUI::Button::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (GUI::Element::ParseXmlAttr(attr, cache))
		return(true);

	TextureManager& tm = cache.getTextureManager();

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

		texture_active = tm.Register(cache.getFileManager(), tn);
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

		texture_hover = tm.Register(cache.getFileManager(), tn);
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

		texture_disabled = tm.Register(cache.getFileManager(), tn);
		return(true);
	}

	return(false);
}

void GUI::Button::Click() {
	Event e(getName(), Event::evtClick, this);
	GUI::Gui::getSingleton().PushEvent(e);
}

void GUI::Button::onGetFocus(){}
void GUI::Button::onLoseFocus(){}


bool GUI::Button::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);
	Click();
	return(true);
}

bool GUI::Button::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	if (!m_enabled)
		return(false);

	if (sdlEvent->key.keysym.sym == SDLK_SPACE || sdlEvent->key.keysym.sym == SDLK_RETURN) {
		Click();
		return(true);
	}
	return(GUI::Element::HandleKeyDown(sdlEvent, mod));
}

void GUI::Button::Draw(unsigned int delay) {
	// Default
	texture = texture_base;

	// Are we the active button?
	if (m_parent != NULL) {
		if (m_parent->getActiveChild() == this) {
			texture = texture_active;
		}

		if ( m_MouseIn ) {
			texture = texture_active;
		}
	}

	Element::Draw();
}
