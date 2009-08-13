/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/event.h"

GUI::Event::Event() {
	m_source = NULL;
}

GUI::Event::Event(const Event& e) {
	m_type = e.m_type;
	m_name = e.m_name;
	m_source = e.m_source;
}

GUI::Event::Event(const std::string& name, Type t, GUI::Element* src) {
	m_name = name;
	m_source = src;
	m_type = t;
}

GUI::Event::~Event() {}

std::string GUI::Event::toString() const {
	std::string ret;
	ret = m_name;
	ret += "::";
	ret += getTypeAsString();
	return(ret);
}

std::string GUI::Event::getTypeAsString() const {
	std::string ret;
	switch(m_type) {
		case evtClick:
			ret = "evtClick";
			break;
		case evtDoubleClick:
			ret = "evtDoubleClick";
			break;
		case evtKeyDown:
			ret = "evtKeyDown";
			break;
		case evtKeyUp:
			ret = "evtKeyUp";
			break;
		case evtMouseIn:
			ret = "evtMouseIn";
			break;
		case evtMouseOut:
			ret = "evtMouseOut";
			break;
		case evtMouseMove:
			ret = "evtMouseMove";
			break;
		case evtDialog:
			ret = "evtDialog";
			break;
		case evtUnknown:
		default:
			ret = "evtUnknown";
	}

	return(ret);
}



std::string GUI::Event::getName() const {
	return(m_name);
}

std::string GUI::Event::getSourceName() const {
	if (m_source == NULL)
		return("");
	return(m_source->getName());
}

GUI::Element* GUI::Event::getSource() {
	return(m_source);
}

const GUI::Element* GUI::Event::getSource() const {
	return(m_source);
}

GUI::Event& GUI::Event::operator =(const Event& e) {
	m_name = e.m_name;
	m_type = e.m_type;
	m_source = e.m_source;

	return(*this);
}

bool GUI::Event::operator == (const Event& e) const {
	return((e.m_name == m_name) && (e.m_type == m_type));
}

bool GUI::Event::operator >= (const Event& e) const {
	if (e.m_name == m_name)
		return(m_type >= m_type);

	return(m_name >= m_name);
}

bool GUI::Event::operator > (const Event& e) const {
	if (e.m_name == m_name)
		return(m_type > m_type);

	return(m_name > m_name);
}

bool GUI::Event::operator <= (const Event& e) const {
	if (e.m_name == m_name)
		return(m_type <= m_type);

	return(m_name <= m_name);
}

bool GUI::Event::operator < (const Event& e) const {
	if (e.m_name == m_name)
		return(m_type < m_type);

	return(m_name < m_name);
}

bool GUI::Event::operator != (const Event& e) const {
	return((e.m_name != m_name) || (e.m_type != m_type));
}
