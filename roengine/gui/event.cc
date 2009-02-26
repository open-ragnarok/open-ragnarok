/* $Id$ */
#include "stdafx.h"

#include "event.h"

GUI::Event::Event() {
	m_source = NULL;
}

GUI::Event::Event(const Event& e) {
	m_name = e.m_name;
	m_source = e.m_source;
}

GUI::Event::Event(const std::string& name, GUI::Element* src) {
	m_name = name;
	m_source = src;
}

GUI::Event::~Event() {}

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
	m_source = e.m_source;

	return(*this);
}
