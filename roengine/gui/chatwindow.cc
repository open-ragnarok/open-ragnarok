#include "stdafx.h"

#include "roengine/gui/chatwindow.h"
#include "roengine/gui/gui.h"

namespace GUI {

ChatWindow::ChatWindow(Element* parent, CacheManager& cache) : Element(parent) {
}

ChatWindow::ChatWindow(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	w = 200;
	h = 122;

	if (node != NULL)
		ParseFromXml(node, cache);

	if (!tex_start.Valid())
		tex_start = LoadTexture("basic_interface\\chatwin1_left.bmp", cache);
//		tex_start = LoadTexture("basic_interface\\chatwin1_line.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("basic_interface\\chatwin1_mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("basic_interface\\chatwin1_right.bmp", cache);

	if (!title_start.Valid())
		title_start = LoadTexture("basic_interface\\titlebar_left.bmp", cache);
	if (!title_mid.Valid())
		title_mid = LoadTexture("basic_interface\\titlebar_mid.bmp", cache);
	if (!title_end.Valid())
		title_end = LoadTexture("basic_interface\\titlebar_right.bmp", cache);
}

ChatWindow::~ChatWindow() {} 

void ChatWindow::Add(std::string s) {
	m_text.push_back(s);
}

void ChatWindow::Clear() {
	m_text.clear();
}

void ChatWindow::Draw(unsigned int delay) {
	float x = 0;
	float y = 0;

	// Sanity check
	if (!tex_start.Valid())
		return;
	if (!tex_end.Valid())
		return;
	if (!tex_mid.Valid())
		return;

	if (!title_start.Valid())
		return;
	if (!title_end.Valid())
		return;
	if (!title_mid.Valid())
		return;

	WindowSeq(x, y, (float)w, (float)h, title_start, title_mid, title_end);
	y += title_start.getHeight();
	WindowSeq(x, y, (float)w, (float)h, tex_start, tex_mid, tex_end);

	x += 25.0f;
	y += 4.0f;
	
	std::vector<std::string>::iterator itr = m_text.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	glColor4f(0.0f,0.0f,0.0f, color[3]);
	while (itr != m_text.end()) {
		gui.textOut(*itr, x, y, 0, 0);
		y += 12.0f;
		itr++;
	}
	glColor4fv(color);

}

}