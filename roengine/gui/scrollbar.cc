/* $Id: scrollbar.cc $ */
#include "stdafx.h"

#include "roengine/gui/scrollbar.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

namespace GUI {

ScrollBar::ScrollBar(Element* parent, CacheManager& cache) : Element(parent) {
	m_value = 0;
	m_maxvalue = 0;

/*	if (!tex_start.Valid())
		tex_start = LoadTexture("scroll0up.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("scroll0mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("scroll0down.bmp", cache);

	if (!tex_bar_start.Valid())
		tex_bar_start = LoadTexture("scroll0bar_up.bmp", cache);
	if (!tex_bar_mid.Valid())
		tex_bar_mid = LoadTexture("scroll0bar_mid.bmp", cache);
	if (!tex_bar_end.Valid())
		tex_bar_end = LoadTexture("scroll0bar_down.bmp", cache);
*/
		tex_start = LoadTexture("scroll0up.bmp", cache);
		tex_mid = LoadTexture("scroll0mid.bmp", cache);
		tex_end = LoadTexture("scroll0down.bmp", cache);

		tex_bar_start = LoadTexture("scroll0bar_up.bmp", cache);
		tex_bar_mid = LoadTexture("scroll0bar_mid.bmp", cache);
		tex_bar_end = LoadTexture("scroll0bar_down.bmp", cache);
}
/*
ScrollBar::ScrollBar(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end) : Element(parent) {
	tex_start = start;
	tex_mid = mid;
	tex_end = end;
	m_value = 0;
	m_maxvalue = 0;
}

ScrollBar::ScrollBar(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager& cache) : Element(parent) {
	tex_start = cache.getTextureManager().Register(cache.getFileManager(), start);
	tex_mid = cache.getTextureManager().Register(cache.getFileManager(), mid);
	tex_end = cache.getTextureManager().Register(cache.getFileManager(), end);
	m_value = 0;
	m_maxvalue = 0;
}*/

ScrollBar::ScrollBar(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	if (node != NULL)
		ParseFromXml(node, cache);

	if (!tex_start.Valid())
		tex_start = LoadTexture("scroll0up.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("scroll0mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("scroll0down.bmp", cache);

	if (!tex_bar_start.Valid())
		tex_bar_start = LoadTexture("scroll0bar_up.bmp", cache);
	if (!tex_bar_mid.Valid())
		tex_bar_mid = LoadTexture("scroll0bar_mid.bmp", cache);
	if (!tex_bar_end.Valid())
		tex_bar_end = LoadTexture("scroll0bar_down.bmp", cache);
}

bool ScrollBar::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (Element::ParseXmlAttr(attr, cache))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "start") {
		tex_start = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "mid") {
		tex_mid = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "end") {
		tex_end = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "val" || attrname == "value" || attrname == "v") {
		if (attr->QueryIntValue(&m_value) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "maxvalue" || attrname == "max" || attrname == "maxval") {
		if (attr->QueryIntValue(&m_maxvalue) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	return(false);
}

void ScrollBar::Draw(unsigned int delay) {
	int bar_h = 0;
	int used_h = 0;

//	this->h  = texture.getHeight();
	if (this->h == 0)
		this->h = tex_mid.getHeight();

	// Draw the background (if any)
	if (texture.Valid())
		Element::Draw();

	// Check all textures
	if (!tex_start.Valid())
		return;
	if (!tex_mid.Valid())
		return;
	if (!tex_end.Valid())
		return;

/*	if (m_value <= 0) {
		return;
	}*/

	if (m_value >= m_maxvalue) {
		bar_h = this->h;
	}
	else {
		float aux = (float)m_value/(float)m_maxvalue;
		bar_h = (int)(aux * this->h);
	}

	if (bar_h == 0)
		return;

	// Upper arrow
	Window((float)0, (float)0, (float)tex_start.getWidth(), (float)tex_start.getHeight(), tex_start);
	used_h = tex_start.getHeight();
	if (used_h >= bar_h)
		return;

/*	if (used_h <= (bar_h + tex_end.getWidth())) {
		int bar_height = bar_h - used_h - tex_end.getHeight();
		for (int i = 0; i < bar_height; i++) {
	//	while (available_y < 0) {
			Window((float)0, (float)(used_h + i), (float)tex_mid.getWidth(), tex_mid.getHeight(), tex_mid);
		}
		used_h += bar_height;
	}*/

	int available_y = h - used_h - tex_end.getHeight();

	// Draw it.
	if (used_h <= (bar_h + tex_end.getWidth())) {
		int bar_height = bar_h - used_h - tex_end.getHeight();
	//	while (available_y > 0) {
		while (available_y > tex_mid.getHeight()) {
			Window(0, used_h - 1, (float)tex_mid.getWidth(), (float)tex_mid.getHeight(), tex_mid);
			used_h += tex_mid.getHeight();
			available_y -= tex_mid.getHeight();
		}
		Window(0, used_h - (tex_mid.getHeight() - available_y), (float)tex_mid.getWidth(), (float)tex_mid.getHeight(), tex_mid);
	}

	// Lower arrow
//	Window((float)0, (float)used_h - 2, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);
	Window((float)0, (float)used_h + available_y - 2, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);





	// Draw bar
	// Upper 
	Window((float)0, (float)tex_start.getHeight() - 1, (float)tex_bar_start.getWidth(), (float)tex_bar_start.getHeight(), tex_bar_start);
	used_h = tex_start.getHeight() + tex_bar_start.getHeight();
	if (used_h >= bar_h)
		return;

/*	if (used_h <= (bar_h + tex_end.getWidth())) {
		int bar_height = bar_h - used_h - tex_end.getHeight();
		for (int i = 0; i < bar_height; i++) {
	//	while (available_y < 0) {
			Window((float)0, (float)(used_h + i), (float)tex_mid.getWidth(), tex_mid.getHeight(), tex_mid);
		}
		used_h += bar_height;
	}*/

	available_y = h - used_h - tex_bar_end.getHeight();

	// Draw it.
	if (used_h <= (bar_h + tex_bar_end.getWidth())) {
		int bar_height = bar_h - used_h - tex_bar_end.getHeight() - tex_end.getHeight();
	//	while (available_y > 0) {
	//	while (available_y > tex_bar_mid.getHeight()) {
			Window(0, used_h - 1, (float)tex_bar_mid.getWidth(), (float)tex_bar_mid.getHeight(), tex_bar_mid);
			used_h += tex_bar_mid.getHeight();
			available_y -= tex_bar_mid.getHeight();
	//	}
	//	Window(0, used_h - (tex_bar_mid.getHeight() - available_y), (float)tex_bar_mid.getWidth(), (float)tex_bar_mid.getHeight(), tex_bar_mid);
	}

	// Lower 
//	Window((float)0, (float)used_h - 2, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
//	Window((float)0, (float)used_h + available_y - 2, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
	Window((float)0, (float)used_h - 1, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
}

void ScrollBar::setValue(int value) {
	m_value = value;
}

void ScrollBar::setMaxValue(int maxvalue) {
	m_maxvalue = maxvalue;
}

int ScrollBar::getValue() const {
	return(m_value);
}

int ScrollBar::getMaxValue() const {
	return(m_maxvalue);
}


}
