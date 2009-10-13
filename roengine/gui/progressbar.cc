/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/progressbar.h"
#include "roengine/gui/event.h"
#include "roengine/gui/gui.h"

namespace GUI {

ProgressBar::ProgressBar(Element* parent) : Element(parent) {
	m_value = 0;
	m_maxvalue = 0;
}

ProgressBar::ProgressBar(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end) : Element(parent) {
	texture_start = start;
	texture_mid = mid;
	texture_end = end;
	m_value = 0;
	m_maxvalue = 0;
}

ProgressBar::ProgressBar(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager& cache) : Element(parent) {
	texture_start = cache.getTextureManager().Register(cache.getFileManager(), start);
	texture_mid = cache.getTextureManager().Register(cache.getFileManager(), mid);
	texture_end = cache.getTextureManager().Register(cache.getFileManager(), end);
	m_value = 0;
	m_maxvalue = 0;
}

ProgressBar::ProgressBar(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent) {
	m_value = 0;
	m_maxvalue = 0;

	if (node != NULL)
		ParseFromXml(node, cache);
}

bool ProgressBar::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	if (Element::ParseXmlAttr(attr, cache))
		return(true);

	std::string attrname = attr->Name();

	if (attrname == "start") {
		texture_start = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "mid") {
		texture_mid = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "end") {
		texture_end = LoadTexture(attr->Value(), cache);
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

void ProgressBar::Draw(unsigned int delay) {
	int bar_w = 0;
	int used_w = 0;

	this->h  = texture.getHeight();
	if (this->h == 0)
		this->h = texture_mid.getHeight();

	// Draw the background (if any)
	if (texture.Valid())
		Element::Draw();

	// Check all textures
	if (!texture_start.Valid())
		return;
	if (!texture_mid.Valid())
		return;
	if (!texture_end.Valid())
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

	Window((float)0, (float)0, (float)texture_start.getWidth(), (float)texture_start.getHeight(), texture_start);
	used_w = texture_start.getWidth();
	if (used_w >= bar_w)
		return;

	if (used_w <= (bar_w + texture_end.getWidth())) {
		int bar_width = bar_w - used_w - texture_end.getWidth();
		for (int i = 0; i < bar_width; i++) {
			Window((float)(used_w + i), (float)0, 1.0f, (float)h, texture_mid);
		}
		used_w += bar_width;
	}

	Window((float)(used_w), (float)0, (float)texture_end.getWidth(), (float)texture_end.getHeight(), texture_end);
}

void ProgressBar::setValue(int value) {
	m_value = value;
}

void ProgressBar::setMaxValue(int maxvalue) {
	m_maxvalue = maxvalue;
}

int ProgressBar::getValue() const {
	return(m_value);
}

int ProgressBar::getMaxValue() const {
	return(m_maxvalue);
}


}
