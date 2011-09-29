/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/list.h"
#include "roengine/gui/gui.h"

GUI::List::List(GUI::Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	if (node != NULL)
		ParseFromXml(node, cache);
	selected = 0;
	m_first = 0;
	m_focusable = true;

	scrollbar = new ScrollBar(this, cache);
	scrollbar->setSize(20, h);
	scrollbar->setPos(w - 5, 0);
	m_children.push_back(scrollbar);
}

bool GUI::List::HandleKeyDown(SDL_Event  *sdlEvent, int mod) {

	SDLKey key = sdlEvent->key.keysym.sym;
	if (key == SDLK_DOWN) {
		selected++;
		if ((unsigned int)selected >= m_items.size())
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

	return(GUI::Element::HandleKeyDown(sdlEvent, mod));
}

bool GUI::List::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);
	//std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (button == 1) {
		unsigned int itm = y / 18;

		if ((itm + m_first) < m_items.size())
			selected = itm + m_first;
	}
	else if (button == 4) {
		selected--;
		if (selected < 0)
			selected = 0;
		if (selected < m_first)
			m_first = selected;
	}
	else if (button == 5) {
		selected++;
		if ((unsigned int)selected >= m_items.size())
			selected = m_items.size() - 1;
	}

	if (m_parent != NULL) {
		if (m_parent->getActiveChild() != NULL) {
			//m_parent->getActiveChild()->onLoseFocus();
			m_parent->setActiveChild(this);
			//onGetFocus();
			GUI::Gui& gui = GUI::Gui::getSingleton();
			gui.setFocus(this);
		}
		else {
			return(false);
		}
	}
	else {
		return(false);
	}

	return(true);
}


void GUI::List::Draw(unsigned int delay) {
	if (m_items.size() == 0)
		return;

	int cy = 0;
	unsigned int i;
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const sdle::Font* font = gui.getDefaultFont();

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	glColor4f(0, 0, 0, color[3]);

	for (i = m_first; i < m_items.size(); i++) {
		if (i == selected) {
			glColor4f(0.482352941f, 0.580392157f, 0.803921569f, color[3] * 0.8f);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glVertex3i(0, cy-1, 0);
			glVertex3i(0 + getW(), cy-1, 0);
			glVertex3i(0 + getW(), cy+17, 0);
			glVertex3i(0, cy+17, 0);
			glEnd();
			glEnable(GL_TEXTURE_2D);
			glColor4f(0, 0, 0, color[3]);
		}
		const std::string &s = m_items[i];
		unsigned int start = 0, x = 0;
		for (unsigned int j = 0; j < s.size(); j++) {
			if (s.c_str()[j] == '^') {
				if (j - start > 0) {
					const std::string ss = s.substr(start, j - start);
					gui.textOut(ss, pos_x + 2 + x, cy + 4, 0, 0);
					x += gui.calcTextWidth(ss.c_str());
				}
				const std::string s1 = s.substr(j + 1, 6);
				unsigned int a = strtol(s1.c_str(), NULL, 16);
				glColor3ub(a >> 16, (a >> 8) & 0xFF, a & 0xFF);
				j += 6;
				start = j + 1;
			}
		}
		if (start < s.size()) {
			const std::string sss = s.substr(start, s.size() - start);
			gui.textOut(sss, pos_x + 2 + x, cy + 4, 0, 0);
		}
//		font->drawText((float)pos_x + 2, (float)cy + 4, m_items[i].c_str());
		cy += 17;
	}

	glColor4fv(color);
}

int GUI::List::getSelected() const {
	if (m_items.size() == 0)
		return(-1);
	return(selected);
}
void GUI::List::setSelected(const int& n) {
	if (n < 0 || (unsigned int)n >= m_items.size()) {
		selected = -1;
		return;
	}
	selected = n;
}

void GUI::List::add(const std::string& s) {
	m_items.push_back(s);
	if (selected == -1)
		selected = 0;
}

void GUI::List::clear() {
	if (m_items.size() > 0)
		m_items.clear();
	selected = -1;
}

void GUI::List::onGetFocus()
{
	actived = true;
}

void GUI::List::onLoseFocus()
{
	actived = false;
}
