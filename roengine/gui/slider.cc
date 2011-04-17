/* $Id: slider.cc $ */
#include "stdafx.h"

#include "roengine/gui/slider.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

namespace GUI {

Slider::Slider(Element* parent, CacheManager& cache) : Element(parent) {
	m_value = 0;
	m_maxvalue = 0;

/*	if (!tex_start.Valid())
		tex_start = LoadTexture("scroll0up.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("scroll0mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("scroll0down.bmp", cache);

	if (!tex_bar_start.Valid())
		tex_bar_start = LoadTexture("scroll0bar_left.bmp", cache);
	if (!tex_bar_mid.Valid())
		tex_bar_mid = LoadTexture("scroll0bar_mid.bmp", cache);
	if (!tex_bar_end.Valid())
		tex_bar_end = LoadTexture("scroll0bar_right.bmp", cache);
*/
		tex_start = LoadTexture("scroll1left.bmp", cache);
		tex_mid = LoadTexture("scroll1mid.bmp", cache);
		tex_end = LoadTexture("scroll1right.bmp", cache);

		tex_bar_start = LoadTexture("scroll1bar_left.bmp", cache);
//		tex_bar_mid = LoadTexture("scroll1bar_mid.bmp", cache);
		tex_bar_end = LoadTexture("scroll1bar_right.bmp", cache);
}
/*
Slider::Slider(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end) : Element(parent) {
	tex_start = start;
	tex_mid = mid;
	tex_end = end;
	m_value = 1;
	m_maxvalue = 1;
}

Slider::Slider(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager& cache) : Element(parent) {
	tex_start = cache.getTextureManager().Register(cache.getFileManager(), start);
	tex_mid = cache.getTextureManager().Register(cache.getFileManager(), mid);
	tex_end = cache.getTextureManager().Register(cache.getFileManager(), end);
	m_value = 1;
	m_maxvalue = 1;
}*/

Slider::Slider(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	if (node != NULL)
		ParseFromXml(node, cache);

	if (!tex_start.Valid())
		tex_start = LoadTexture("scroll1left.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("scroll1mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("scroll1right.bmp", cache);

	if (!tex_bar_start.Valid())
		tex_bar_start = LoadTexture("scroll1bar_left.bmp", cache);
//	if (!tex_bar_mid.Valid())
//		tex_bar_mid = LoadTexture("scroll1bar_mid.bmp", cache);
	if (!tex_bar_end.Valid())
		tex_bar_end = LoadTexture("scroll1bar_right.bmp", cache);
}

bool Slider::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
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

	if (!tex_start.Valid())
		tex_start = LoadTexture("scroll1left.bmp", cache);
	if (!tex_mid.Valid())
		tex_mid = LoadTexture("scroll1mid.bmp", cache);
	if (!tex_end.Valid())
		tex_end = LoadTexture("scroll1right.bmp", cache);

	if (!tex_bar_start.Valid())
		tex_bar_start = LoadTexture("scroll1bar_left.bmp", cache);
//	if (!tex_bar_mid.Valid())
//		tex_bar_mid = LoadTexture("scroll1bar_mid.bmp", cache);
	if (!tex_bar_end.Valid())
		tex_bar_end = LoadTexture("scroll1bar_right.bmp", cache);

	return(false);
}
/*
void Slider::Draw(unsigned int delay) {
	int bar_w = 0;
	int used_w = 0;

	this->w  = texture.getHeight();
	if (this->w == 0)
		this->w = tex_mid.getHeight();

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

/*	if (m_value >= m_maxvalue) {
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
			Window((float)(used_w + i), (float)0, 1.0f, (float)w, tex_mid);
		}
		used_w += bar_width;
	}

	Window((float)(used_w), (float)0, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);
}*/

void Slider::Draw(unsigned int delay) {
	int bar_w = 0;
	int used_w = 0;

//	this->w  = texture.getHeight();
	if (this->w == 0)
		this->w = tex_mid.getWidth();

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
		bar_w = this->w;
	}
	else {
		float aux = (float)m_value/(float)m_maxvalue;
		bar_w = (int)(aux * this->w);
	}

	if (bar_w == 0)
		return;

	// Upper arrow
	Window((float)0, (float)0, (float)tex_start.getWidth(), (float)tex_start.getHeight(), tex_start);
	used_w = tex_start.getWidth();
//	if (used_w >= bar_w)
//		return;

/*	if (used_w <= (bar_w + tex_end.getWidth())) {
		int bar_weight = bar_w - used_w - tex_end.getHeight();
		for (int i = 0; i < bar_weight; i++) {
	//	while (available_x < 0) {
			Window((float)0, (float)(used_w + i), (float)tex_mid.getWidth(), tex_mid.getHeight(), tex_mid);
		}
		used_w += bar_weight;
	}*/

	int available_x = w - used_w - tex_end.getWidth();

	// Draw it.
	if (used_w <= (bar_w + tex_end.getWidth())) {
		int bar_weight = bar_w - used_w - tex_end.getWidth();
	//	while (available_x > 0) {
		while (available_x > tex_mid.getWidth()) {
			Window(used_w - 1, 0, (float)tex_mid.getWidth(), (float)tex_mid.getHeight(), tex_mid);
			used_w += tex_mid.getWidth();
			available_x -= tex_mid.getWidth();
		}
		Window(used_w - (tex_mid.getWidth() - available_x), 0, (float)tex_mid.getWidth(), (float)tex_mid.getHeight(), tex_mid);
	}

	// Lower arrow
//	Window((float)0, (float)used_w - 2, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);
	Window((float)used_w + available_x - 2, (float)0, (float)tex_end.getWidth(), (float)tex_end.getHeight(), tex_end);

	// Draw bar
	// Upper 
	Window((float)tex_start.getWidth() - 1, (float)0, (float)tex_bar_start.getWidth(), (float)tex_bar_start.getHeight(), tex_bar_start);
	used_w = tex_start.getWidth() + tex_bar_start.getWidth();
	if (used_w >= bar_w)
		return;

/*	if (used_w <= (bar_w + tex_end.getWidth())) {
		int bar_weight = bar_w - used_w - tex_end.getHeight();
		for (int i = 0; i < bar_weight; i++) {
	//	while (available_x < 0) {
			Window((float)0, (float)(used_w + i), (float)tex_mid.getWidth(), tex_mid.getHeight(), tex_mid);
		}
		used_w += bar_weight;
	}*/

	available_x = w - used_w - tex_bar_end.getWidth();

	// Draw it.
/*	if (used_w <= (bar_w + tex_bar_end.getWidth())) {
		int bar_weight = bar_w - used_w - tex_bar_end.getHeight() - tex_end.getHeight();
	//	while (available_x > 0) {
	//	while (available_x > tex_bar_mid.getHeight()) {
			Window(0, used_w - 1, (float)tex_bar_mid.getWidth(), (float)tex_bar_mid.getHeight(), tex_bar_mid);
			used_w += tex_bar_mid.getHeight();
			available_x -= tex_bar_mid.getHeight();
	//	}
	//	Window(0, used_w - (tex_bar_mid.getHeight() - available_x), (float)tex_bar_mid.getWidth(), (float)tex_bar_mid.getHeight(), tex_bar_mid);
	}*/

	// Lower 
//	Window((float)0, (float)used_w - 2, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
//	Window((float)0, (float)used_w + available_x - 2, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
	Window((float)used_w - 1, (float)0, (float)tex_bar_end.getWidth(), (float)tex_bar_end.getHeight(), tex_bar_end);
}

void Slider::setValue(int value) {
	m_value = value;
}

void Slider::setMaxValue(int maxvalue) {
	m_maxvalue = maxvalue;
}

int Slider::getValue() const {
	return(m_value);
}

int Slider::getMaxValue() const {
	return(m_maxvalue);
}


}
