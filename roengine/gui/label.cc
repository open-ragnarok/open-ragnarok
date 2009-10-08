/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/label.h"
#include "roengine/gui/gui.h"

#include <GL/gl.h>
#include <GL/glu.h>

GUI::Label::Label(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	m_focusable = false;
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

	glColor3f(0.0f,0.0f,0.0f);
	gui.textOut(m_text, 0, 0, 0, MaxLen);
	glColor3f(1.0f,1.0f,1.0f);
}

bool GUI::Label::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (GUI::Element::ParseXmlAttr(attr, cache))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "text") {
		m_text = attr->Value();
		return(true);
	}
	return(false);
}
