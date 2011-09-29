/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/label.h"
#include "roengine/gui/gui.h"

GUI::Label::Label(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	m_focusable = false;
	m_shadow = false;
	m_rightalign = false;

	if (node != NULL)
		ParseFromXml(node, cache);
}

GUI::Label& GUI::Label::setText(const std::string& s) {
	m_text = s;
	return(*this);
}

GUI::Label& GUI::Label::setText(const char* s) {
	m_text = s;
	return(*this);
}

std::string& GUI::Label::getText() {
	return(m_text);
}

const std::string& GUI::Label::getText() const {
	return(m_text);
}

void GUI::Label::Draw(unsigned int delay) {
	GUI::Gui& gui = GUI::Gui::getSingleton();
	//const GUI::Font* font = gui.getDefaultFont();

	int x = 0;
	if (m_rightalign)
		x = w - gui.calcTextWidth(m_text.c_str()) - 1;

	if (m_shadow) {
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	//	glColor4f(1.0f,1.0f,1.0f,m_parent->m_opacity);
		gui.textOut(m_text, x + 1.f, 1.f, 0.f, MaxLen);
	}
	glColor3f(0.0f,0.0f,0.0f);
//	glColor4f(0.0f,0.0f,0.0f,m_parent->m_opacity);
	gui.textOut(m_text, x, 0, 0, MaxLen);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

bool GUI::Label::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (GUI::Element::ParseXmlAttr(attr, cache))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "text") {
		m_text = attr->Value();
		return(true);
	}
	if (attrname == "shadow") {
		std::string value = attr->Value();
		if ((value == "true") || (value == "1"))
			m_shadow = true;
		else
			m_shadow = false;
		return(true);
	}
	if (attrname == "rightalign") {
		std::string value = attr->Value();
		if ((value == "true") || (value == "1"))
			m_rightalign = true;
		else
			m_rightalign = false;
		return(true);
	}

	return(false);
}
