/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/label.h"
#include "roengine/gui/gui.h"

#include <GL/gl.h>
#include <GL/glu.h>

GUI::Label::Label(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) : Element(parent) {
	m_focusable = false;
	if (node != NULL)
		ParseFromXml(node, tm, fm);
}

GUI::Label& GUI::Label::setText(const std::string& s) {
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
	const GUI::Font* font = gui.getDefaultFont();

	glColor3f(0,0,0);
	font->textOut(m_text, (float)pos_x, (float)pos_y, 0);
	glColor3f(1,1,1);
}

bool GUI::Label::ParseXmlAttr(const TiXmlAttribute* attr, TextureManager& tm, FileManager& fm) {
	if (GUI::Element::ParseXmlAttr(attr, tm, fm))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "text") {
		m_text = attr->Value();
		return(true);
	}
	return(false);
}
