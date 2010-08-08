/* $Id: statuswindow.cc $ */
#include "stdafx.h"

#include "roengine/gui/StatusWindow.h"
#include "roengine/gui/gui.h"

namespace GUI {

StatusWindow::StatusWindow(Element* parent, CacheManager& cache) : Element(parent) {
}

StatusWindow::StatusWindow(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	if (node != NULL)
		ParseFromXml(node, cache);

	if (!title_l.Valid())
		title_l = LoadTexture("basic_interface\\titlebar_left.bmp", cache);
	if (!title_m.Valid())
		title_m = LoadTexture("basic_interface\\titlebar_mid.bmp", cache);
	if (!title_r.Valid())
		title_r = LoadTexture("basic_interface\\titlebar_right.bmp", cache);

	if (!tex_1.Valid())
		tex_1 = LoadTexture("basic_interface\\statwin0_bg.bmp", cache);
	if (!tex_2.Valid())
		tex_2 = LoadTexture("basic_interface\\statwin1_bg.bmp", cache);
}

StatusWindow::~StatusWindow() {} 

void StatusWindow::Add(std::string s) {
//	m_text.push_back(s);
}

void StatusWindow::Clear() {
//	m_text.clear();
}

void StatusWindow::Draw(unsigned int delay) {
	float x = 0;
	float y = 0;

	// Sanity check
	if (!title_l.Valid())
		return;
	if (!title_m.Valid())
		return;
	if (!title_r.Valid())
		return;

	if (!tex_1.Valid())
		return;
	if (!tex_2.Valid())
		return;

	x += 440;
	y += 317.0f;
	WindowSeq(x, y, (float)w, (float)h, title_l, title_m, title_r);

	y += title_l.getHeight();
	Window(x, y, w, h, tex_1);

	x += 25.0f;
	y += 4.0f;
	
/*	std::vector<std::string>::iterator itr = m_text.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	glColor4f(0.0f,0.0f,0.0f, color[3]);
	glColor4fv(color);*/

}

}