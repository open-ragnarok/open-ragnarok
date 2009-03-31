/* $Id$ */
#include "stdafx.h"

#include "list.h"
#include "gui.h"

#include <SDL.h>

GUI::List::List(GUI::Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) : Element(parent) {
	if (node != NULL)
		ParseFromXml(node, tm, fm);
	selected = 0;
	m_first = 0;
}

bool GUI::List::HandleKeyDown(int key, int mod) {
	if (key == SDLK_DOWN) {
		selected++;
		if (selected >= m_items.size())
			selected = m_items.size() - 1;
		return(true);
	}

	if (key == SDLK_UP) {
		selected--;
		if (selected < 0)
			selected = 0;
		if (selected < m_first)
			m_first = selected;
		return(true);
	}

	if (key == SDLK_HOME) {
		selected = 0;
		m_first = 0;
		return(true);
	}

	if (key == SDLK_END) {
		selected = m_items.size() - 1;
		return(true);
	}

	return(GUI::Element::HandleKeyDown(key, mod));
}

bool GUI::List::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);
	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	int itm = y / 18;

	if ((itm + m_first) < m_items.size())
		selected = itm + m_first;
	
	return(true);
}


void GUI::List::Draw(unsigned int delay) {
	if (m_items.size() == 0)
		return;

	int cy = pos_y;
	int i;
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const GUI::Font* font = gui.getDefaultFont();
	glColor3f(0,0,0);

	for (i = m_first; i < m_items.size(); i++) {
		if (i == selected) {
			glColor3f(0.482352941, 0.580392157, 0.803921569);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glVertex3f(pos_x, cy-1, 0);
			glVertex3f(pos_x + getW(), cy-1, 0);
			glVertex3f(pos_x + getW(), cy+17, 0);
			glVertex3f(pos_x, cy+17, 0);
			glEnd();
			glEnable(GL_TEXTURE_2D);
			glColor3f(0, 0, 0);
		}
		font->textOut(m_items[i], (float)pos_x + 2, (float)cy, 0);
		cy += 17;
	}

	glColor3f(1,1,1);
}

int GUI::List::getSelected() const {
	if (m_items.size() == 0)
		return(-1);
	return(selected);
}
void GUI::List::setSelected(const int& n) {
	if (n < 0) {
		selected = -1;
		return;
	}
}

void GUI::List::add(const std::string& s) {
	m_items.push_back(s);
}

void GUI::List::clear() {
	if (m_items.size() > 0)
		m_items.clear();
	selected = -1;
}
