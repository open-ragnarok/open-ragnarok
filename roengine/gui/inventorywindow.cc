/* $Id: inventorywindow.cc $ */
#include "stdafx.h"

#include "roengine/gui/inventorywindow.h"
#include "roengine/gui/gui.h"

namespace GUI {

InventoryWindow::InventoryWindow(Element* parent, CacheManager& cache) : Element(parent) {
}

InventoryWindow::InventoryWindow(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
//	w = 278;//200;
//	h = 178;//122;

	if (node != NULL)
		ParseFromXml(node, cache);

	if (!tex_l.Valid())
		tex_l = LoadTexture("basic_interface\\itemwin_left.bmp", cache);
	if (!tex_m.Valid())
		tex_m = LoadTexture("basic_interface\\itemwin_mid.bmp", cache);
	if (!tex_invert.Valid())
		tex_invert = LoadTexture("basic_interface\\item_invert.bmp", cache);
	if (!tex_r.Valid())
		tex_r = LoadTexture("basic_interface\\itemwin_right.bmp", cache);
}

InventoryWindow::~InventoryWindow() {} 

void InventoryWindow::Add(std::string s) {
	m_text.push_back(s);
}

void InventoryWindow::Clear() {
	m_text.clear();
}

void InventoryWindow::WindowSeq(float x, float y, float w, float h, const sdle::Texture& tex) {
	float used_y = 0;
	float available_y;

/*	// Left texture
	Window(x, y, (float)start.getWidth(), (float)start.getHeight(), start);
	used_x += start.getWidth();
	*/
	// Checkout how much space we have
	available_y = h - used_y - tex.getHeight();

	// Draw it.
	while (available_y > 0) {
		Window(x, y + used_y, (float)tex.getWidth(), (float)tex.getHeight(), tex);
		used_y += tex.getHeight();
		available_y -= tex.getHeight();
	}

	// Right edge
//	Window(w - end.getWidth(), y, (float)end.getWidth(), (float)end.getHeight(), end);
}

void InventoryWindow::WindowSeq(float x, float y, float w, float h, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end) {
	float used_x = 0;
	float available_x;

	// Left texture
	WindowSeq(x, y, w, h, start);
	used_x += start.getWidth();
	
	// Checkout how much space we have
	available_x = w - used_x - end.getWidth();

	// Draw it.
	while (available_x > 0) {
		WindowSeq(x + used_x, y, w, h, mid);
		used_x += mid.getWidth();
		available_x -= mid.getWidth();
	}

	// Right edge
	WindowSeq(w - end.getWidth(), y, w, h, end);
}
/*
void WindowSeq(float x, float y, float w, float h, 
			   const sdle::Texture& lu, const sdle::Texture& lm, const sdle::Texture& ld,
			   const sdle::Texture& mu, const sdle::Texture& mm, const sdle::Texture& md,
			   const sdle::Texture& ru, const sdle::Texture& rm, const sdle::Texture& rd) {
	float used_y = 0;
	float available_y;

	// Top line
	WindowSeq(x, y, w, h, lu, mu, ru);
	used_y += lu.getHeight();
	
	// Checkout how much space we have
	available_y = h - used_y - ld.getHeight();

	// Draw it.
	while (available_y > 0) {
	//	Window(x+used_y, y, (float)mid.getWidth(), (float)mid.getHeight(), mid);
		WindowSeq(x, y + used_y, w, h, lm, mm, rm);
		used_y += lm.getHeight();
		available_y -= lm.getHeight();
	}

	// Bottom line
//	Window(w - end.getWidth(), y, (float)end.getWidth(), (float)end.getHeight(), end);
	WindowSeq(x, h - ld.getHeight(), w, h, ld, md, rd);
}*/

void InventoryWindow::Draw(unsigned int delay) {
	float x = 0;
	float y = 0;

	// Sanity check
	if (!tex_l.Valid())
		return;
	if (!tex_m.Valid())
		return;
	if (!tex_invert.Valid())
		return;
	if (!tex_r.Valid())
		return;

	float used_y = 0;
	float available_y;

	WindowSeq(x, y, (float)w, (float)h, tex_l, tex_m, tex_r);
/*	while (available_y > 0) {
	//	Window(x+used_y, y, (float)mid.getWidth(), (float)mid.getHeight(), mid);
		WindowSeq(x, y + used_y, w, h, tex_l, tex_m, tex_r);
		used_y += tex_l.getHeight();
		available_y -= tex_l.getHeight();
	}*/
	// Left texture
/*	used_y = 0;
	while (available_y > 0) {
		Window(x, y, (float)tex_l.getWidth(), (float)tex_l.getHeight(), tex_l);
	}
	used_x += tex_l.getWidth();
	
	// Checkout how much space we have
	available_x = w - used_x - end.getWidth();

	// Draw it.
	used_y = 0;
	while (available_x > 0) {
		Window(x+used_x, y, (float)mid.getWidth(), (float)mid.getHeight(), mid);
		used_x += mid.getWidth();
		available_x -= mid.getWidth();
	}

	// Right edge
	used_y = 0;
	while (available_y > 0) {
		Window(w - tex_r.getWidth(), y + used_y, (float)tex_r.getWidth(), (float)tex_r.getHeight(), tex_r);
	}*/

	//x += 25.0f;
	//y += 4.0f;
	x += 14;
	y += 17.0f;
	
	std::vector<std::string>::iterator itr = m_text.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	glColor4f(0.0f,0.0f,0.0f, color[3]);
	glColor4fv(color);

}

}