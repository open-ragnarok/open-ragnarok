/* $Id: checkbox.cc $ */
#include "stdafx.h"

#include "roengine/gui/checkbox.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

namespace GUI {

CheckBox::CheckBox(Element* parent) : Button(parent) {
	m_checked = false;
}

CheckBox::CheckBox(Element* parent, const sdle::Texture& tex_base, const sdle::Texture& tex_checked) : Button(parent) {
	m_checked = false;
}

CheckBox::CheckBox(Element* parent, const std::string& base, const std::string& checked, CacheManager& cache) : Button(parent) {
	m_checked = false;
}

CheckBox::CheckBox(Element* parent, const TiXmlElement* node, CacheManager& cache) : Button(parent) {
	m_checked = false;

	if (node != NULL)
		ParseFromXml(node, cache);

	texture_base = texture;
}

bool CheckBox::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (Element::ParseXmlAttr(attr, cache))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "active") {
		texture_checked = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "checked") {
		std::string value = attr->Value();
		if ((value == "true") || (value == "1"))
			m_checked = true;
		else
			m_checked = false;
		return(true);
	}
	return(false);
}

void CheckBox::Draw(unsigned int delay) {
	// Check all textures
	if (!texture_base.Valid())
		return;
	if (!texture_checked.Valid())
		return;

	if (m_checked)
		texture = texture_checked;
	else
		texture = texture_base;

	Element::Draw();

}

void CheckBox::setChecked(bool checked) {
	m_checked = !checked;
}

bool CheckBox::isChecked() const {
	return(m_checked);
}


}
