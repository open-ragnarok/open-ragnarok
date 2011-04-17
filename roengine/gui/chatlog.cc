/* $Id: chatlog.cc  $ */
#include "stdafx.h"

#include "roengine/gui/chatlog.h"
#include "roengine/gui/gui.h"

namespace GUI {

ChatLog::ChatLog(Element* parent, CacheManager& cache) : Element(parent) {
}

ChatLog::ChatLog(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	if (node != NULL)
		ParseFromXml(node, cache);
}

ChatLog::~ChatLog() {} 

void ChatLog::Add(std::string s) {
	m_text.push_back(s);
}

void ChatLog::Clear() {
	m_text.clear();
}

void ChatLog::Draw(unsigned int delay) {
	float x = 0;
	float y = 0;

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

//	glColor4f(0.3f, 0.3f, 0.8f, color[3]);
	glEnable(GL_BLEND);
	glColor4f(0.3f, 0.3f, 0.3f, 1.f);
	glBegin(GL_QUADS);
	glVertex3f(0,   0,   0);
	glVertex3f(0,   h, 0);
	glVertex3f(w, h, 0);
	glVertex3f(w, 0,   0);
	glEnd();
	glDisable(GL_BLEND);

	glColor4fv(color);

	//x += 25.0f;
	//y += 4.0f;
//	x += 14;
//	y += 17.0f;
	x += 4;//10;
	y += 4;//13;
	
	std::vector<std::string>::iterator itr = m_text.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

//	float color[4];
//	glGetFloatv(GL_CURRENT_COLOR, color);

	glColor4f(0.0f,0.0f,0.0f, color[3]);
	while (itr != m_text.end()) {
		const std::string &s = *itr;
		unsigned int start = 0, c = 0, a = 0, ws = 0;
#if 0
		for (int i = 0; i < s.size(); i++) {
			if (s.c_str()[i] == ' ') {
				if (i - start > 0) {
					const std::string ss = s.substr(start, i - start);
					if ((x + gui.calcTextWidth(ss.c_str())) > 200) {
						const std::string sss = s.substr(start, ws - start);
						gui.textOut(sss, x, y, 0, 0);
					//	x += gui.calcTextWidth(ss.c_str());
						y += 18.0f;
						x = 14;
						start += sss.size();
					}
					else
						ws = i + 1;
				}
				else
					ws = i + 1;
			}
			else if (s.c_str()[i] == '^') {
				if (i - start > 0) {
					const std::string ss = s.substr(start, i - start);
//					x += 6 * (start - 7);
					gui.textOut(ss, x, y, 0, 0);
				//	x += 6 * ss.size();
					x += gui.calcTextWidth(ss.c_str());
				//	const std::string s2 = s.erase(i, 6);
				}
				const std::string s1 = s.substr(i + 1, 6);
				a = strtol(s1.c_str(), NULL, 16);
				glColor3ub(a >> 16, (a >> 8) & 0xFF, a & 0xFF);
				i += 6;
				start = i + 1;
			//	c += i - start;
			}
			else {
//				c++;
			}
		/*	const std::string ss = s.substr(start, ws - start);
			if ((x + gui.calcTextWidth(ss.c_str())) > 175) {
				gui.textOut(ss, x, y, 0, 0);
			//	x += gui.calcTextWidth(ss.c_str());
				y += 18.0f;
				x = 14;
				start += ss.size();
			}*/

		/*	if (c > 0 && (c % 48) == 0) {
				c = 0;
				const std::string ss = s.substr(start, i - start);
			//	x += 6 * start;
				gui.textOut(ss, x, y, 0, 0);
				y += 18.0f;
				x = 14;
				start += 47;
			}*/
		}
//		if (c < 48 && start < s.size()) {
		if (start < s.size()) {
		//	gui.textOut(*itr, x, y, 0, 0);
			const std::string sss = s.substr(start, s.size() - start);
		//	const std::string sss = s.substr(start, ws - start);
			gui.textOut(sss, x, y, 0, 0);
		}
		x = 14;
		y += 18.0f;
		ws = 0;
#endif
		std::vector<std::string> words;
		
		// Parse line
		for (unsigned int i = 0; i < s.size(); i++) {
			if (s.c_str()[i] == ' ') {
				if (i - start > 0) {
					const std::string ss = s.substr(start, i - start + 1);
					words.push_back(ss);
				//	start += ss.size();
				//	i++;
					start = i + 1;
				}
			}
			else if (s.c_str()[i] == '^') {
				if (i - start > 0) {
					const std::string ss = s.substr(start, i - start);
					words.push_back(ss);
				}
				const std::string s1 = s.substr(i, 7);
				words.push_back(s1);
			//	i += 6;
			//	start = i + 1;
				i += 7;
				start = i;
			}
		}
		if (start < s.size()) {
			const std::string ss = s.substr(start, s.size() - start);
			words.push_back(ss);
		}
		// Draw line
		for (unsigned int i = 0; i < words.size(); i++) {
			const std::string &s = words[i];
			if (s.c_str()[0] == '^') {
				const std::string s1 = s.substr(1, 6);
				a = strtol(s1.c_str(), NULL, 16);
				glColor3ub(a >> 16, (a >> 8) & 0xFF, a & 0xFF);
			}
			else {
				int a = gui.calcTextWidth(s.c_str());
//				if ((x + gui.calcTextWidth(s.c_str())) > 245) { //200) {
				if ((x + gui.calcTextWidth(s.c_str())) > 600) {
					if (gui.calcTextWidth(s.c_str()) < 600) {
						y += 12;//18.0f;
						x = 4;//10;
					}
				}
				glColor3f(1.0f, 1.0f, 1.0f);
				if (y < (7 * 18))
					gui.textOut(s, x, y, 0, 0);
				x += gui.calcTextWidth(s.c_str());
			}
		}
		y += 12;//18.0f;
		x = 4;//10;
		itr++;
	}
	glColor4fv(color);

}

}