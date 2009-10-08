#include "stdafx.h"

#include "roengine/gui/buttonbar.h"

namespace GUI {

ButtonBar::ButtonBar(Element* parent, CacheManager& cache) : Element(parent) {
	tex_left = LoadTexture("basic_interface\\btnbar_left.bmp", cache);
	tex_right = LoadTexture("basic_interface\\btnbar_right.bmp", cache);
	tex_mid = LoadTexture("basic_interface\\btnbar_mid.bmp", cache);
	tex_arrow = LoadTexture("basic_interface\\btnbar_arrow.bmp", cache);

	m_arrow = false;
	m_arrow_begin = false;
	m_MouseIn = false;

	CheckMinimumSize();
}

ButtonBar::ButtonBar(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	m_arrow = false;
	m_arrow_begin = false;
	m_MouseIn = false;

	if (node != NULL)
		ParseFromXml(node, cache);


	if (!tex_left.Valid())
		tex_left = LoadTexture("basic_interface\\btnbar_left.bmp", cache);
	if (!tex_right.Valid())
		tex_right = LoadTexture("basic_interface\\btnbar_right.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("basic_interface\\btnbar_mid.bmp", cache);
	if (!tex_arrow.Valid())
		tex_arrow = LoadTexture("basic_interface\\btnbar_arrow.bmp", cache);

	CheckMinimumSize();
}

ButtonBar::~ButtonBar() {}

void ButtonBar::CheckMinimumSize() {
	int minw = tex_left.getWidth() + tex_right.getWidth();
	if (m_arrow)
		minw += tex_arrow.getWidth();

	h = tex_mid.getHeight();

	if (m_parent != NULL) {
		w = m_parent->getW();
		pos_y = m_parent->getH() - h;
	}

	if (w < minw)
		w = minw;
}

void ButtonBar::Draw(unsigned int delay) {
	CheckMinimumSize();

	float x = 0;
	float y = 0;
	float used_x = 0;
	float available_x;

	// Sanity check
	if (!tex_left.Valid())
		return;
	if (!tex_right.Valid())
		return;
	if (!tex_mid.Valid())
		return;
	if (!tex_arrow.Valid())
		return;

	// Left texture
	Window(x, y, (float)tex_left.getWidth(), (float)tex_left.getHeight(), tex_left);
	used_x += tex_left.getWidth();
	
	// Left arrow
	if (m_arrow && m_arrow_begin) {
		Window(x + used_x, y, (float)tex_arrow.getWidth(), (float)tex_arrow.getHeight(), tex_arrow);
		used_x += tex_arrow.getWidth();
	}

	// Checkout how much space we have
	available_x = w - used_x - tex_right.getWidth();
	if (m_arrow && !m_arrow_begin)
		available_x -= tex_arrow.getWidth();

	// Draw it.
	while (available_x > 0) {
		Window(x+used_x, y, (float)tex_mid.getWidth(), (float)tex_mid.getHeight(), tex_mid);
		used_x += tex_mid.getWidth();
		available_x -= tex_mid.getWidth();
	}

	// Right arrow
	if (m_arrow && !m_arrow_begin) {
		Window(x + used_x, y, (float)tex_arrow.getWidth(), (float)tex_arrow.getHeight(), tex_arrow);
		used_x += tex_arrow.getWidth();
	}

	// Right edge
	Window(w - tex_right.getWidth(), y, (float)tex_right.getWidth(), (float)tex_right.getHeight(), tex_right);

	// Position buttons
	used_x = 0;

	std::vector<Element*>::iterator itr = m_children.begin();
	while(itr != m_children.end()) {
		if ((*itr)->isVisible()) {
			used_x += (*itr)->getW() + 5.0f;
			(*itr)->setPos(w - used_x, 1.0f);
		}
		itr++;
	}
}



}