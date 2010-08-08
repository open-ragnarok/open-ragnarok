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
	int bar_w = 0;
	int used_w = 0;

	this->h  = texture.getHeight();
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

	if (m_value <= 0) {
		return;
	}

	if (m_value >= m_maxvalue) {
		bar_w = this->w;
	}
	else {
		float aux = (float)m_value/(float)m_maxvalue;
		bar_w = (int)(aux * this->w);
	}

	if (bar_w == 0)
		return;

	Window((float)0, (float)0, (float)tex_start.getWidth(), (float)tex_start.getHeight(), tex_start);
	used_w = tex_start.getWidth();
	if (used_w >= bar_w)
		return;

	if (used_w <= (bar_w + tex_end.getWidth())) {
		int bar_width = bar_w - used_w - tex_end.getWidth();
		for (int i = 0; i < bar_width; i++) {
			Window((float)(used_w + i), (float)0, 1.0f, (float)h, tex_mid);
		}
		used_w += bar_width;
	}

	Window((float)(used_w), (float)0, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);
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
