/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/button.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

GUI::Button::Button(Element* parent) : Element(parent) {
	m_mouseIn = false;
	m_mouseDown = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base) : Element(parent) {
	texture_base = base;
	texture_hover = base;
	texture_click = base;
	texture_disabled = base;
	m_mouseIn = false;
	m_mouseDown = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& hover) : Element(parent) {
	texture_base = base;
	texture_hover = hover;
	texture_click = hover;
	texture_disabled = base;
	m_mouseIn = false;
	m_mouseDown = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& hover, const sdle::Texture& click) : Element(parent) {
	texture_base = base;
	texture_hover = hover;
	texture_click = click;
	texture_disabled = base;
	m_mouseIn = false;
	m_mouseDown = false;
}

GUI::Button::Button(Element* parent, const sdle::Texture& base, const sdle::Texture& hover, const sdle::Texture& click, const sdle::Texture& disabled) : Element(parent) {
	texture_base = base;
	texture_hover = hover;
	texture_click = click;
	texture_disabled = disabled;
	m_mouseIn = false;
	m_mouseDown = false;
}

GUI::Button::Button(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	if (node != NULL)
		ParseFromXml(node, cache);

	texture_base = texture;
	m_mouseIn = false;
	m_mouseDown = false;

	if (!texture_hover.Valid())
		texture_hover = texture;
	if (!texture_click.Valid())
//		texture_click = texture;
		texture_click = texture_hover;
	if (!texture_disabled.Valid())
		texture_disabled = texture;
}

GUI::Button::Button(Element* parent, const std::string& background, CacheManager& cache) : Element(parent, background, cache) {
	texture_base = texture;
	texture_hover = texture;
	texture_click = texture;
	texture_disabled = texture;
	m_mouseIn = false;
	m_mouseDown = false;
}

bool GUI::Button::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (GUI::Element::ParseXmlAttr(attr, cache))
		return(true);

	TextureManager& tm = cache.getTextureManager();

	std::string attrname = attr->Name();

	if (attrname == "hover") {
		texture_hover = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "click") {
		texture_click = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "disabled") {
		texture_disabled = LoadTexture(attr->Value(), cache);
		return(true);
	}

	return(false);
}

void GUI::Button::Down() {
	Event e(getName(), Event::evtMouseDown, this);
	GUI::Gui::getSingleton().PushEvent(e);
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

	//std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);
//	Click();
	Down();
//	m_mouseDown = true;
	return(true);
}

bool GUI::Button::HandleMouseRelease(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	if (!(button & 1))
		return(false);

	if (m_mouseDown) {
		Click();
	}
	m_mouseDown = false;
	return(true);
}
/*
bool GUI::Button::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	if (!m_enabled)
		return(false);

	if (sdlEvent->key.keysym.sym == SDLK_SPACE || sdlEvent->key.keysym.sym == SDLK_RETURN) {
		Click();
		return(true);
	}
	return(GUI::Element::HandleKeyDown(sdlEvent, mod));
}*/

void GUI::Button::Draw(unsigned int delay) {
	// Default
	texture = texture_base;

	// Are we the hover button?
	if (m_parent != NULL) {
		if (m_parent->getActiveChild() == this) {
			texture = texture_hover;
		}
	}

	if (m_mouseIn) {
		if (m_mouseDown)
			texture = texture_click;
		else
			texture = texture_hover;
	}

	Element::Draw();
}
