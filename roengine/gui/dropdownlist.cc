/* $Id: dropdownlist.cc $ */
#include "stdafx.h"

#include "roengine/gui/dropdownlist.h"
#include "roengine/gui/gui.h"

namespace GUI {

//DropDownList::DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache) : List(parent, e, cache) {
//DropDownList::DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache) : Button(parent, e, cache) {
/*DropDownList::DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache) 
: Label(parent, e, cache) 
, button(parent, sdle::Texture
, list(parent, e, cache)*/
DropDownList::DropDownList(Element* parent, const TiXmlElement* e, CacheManager& cache) 
{
	if (e != NULL)
		ParseFromXml(e, cache);

	m_text = "<Basic skin>";
	texture = LoadTexture("basic_interface\\txtbox_btn_a.bmp", cache);
}

DropDownList::~DropDownList() {
}

void DropDownList::Add(std::string s) {
	m_items.push_back(s);
	if (selected == -1)
		selected = 0;
}

void DropDownList::Clear() {
	if (m_items.size() > 0)
		m_items.clear();
	selected = -1;
}

/*
void DropDownList::Click() {
	Event e(getName(), Event::evtClick, this);
	GUI::Gui::getSingleton().PushEvent(e);
}*/

bool DropDownList::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	if (!(button & 1))
		return(false);
//	Click();
//	Down();
//	m_mouseDown = true;
	return(true);
}

void DropDownList::Draw(unsigned int delay) {
	GUI::Gui& gui = GUI::Gui::getSingleton();
	//const GUI::Font* font = gui.getDefaultFont();

	int x = 0;
	glColor3f(0.0f,0.0f,0.0f);
//	glColor4f(0.0f,0.0f,0.0f,m_parent->m_opacity);
	gui.textOut(m_text, 2, 3, 0, MaxLen);

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	Window(w - texture.getWidth(), 0, texture.getWidth(), texture.getHeight(), texture);
}

}