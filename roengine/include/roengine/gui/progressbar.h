/* $Id$ */
#ifndef __GUI_PROGRESSBAR_H
#define __GUI_PROGRESSBAR_H

#include "element.h"

namespace GUI {

/**
 * An implementation for a progress bar (used, for example, in the HP and SP bars)
 */
class ProgressBar : public Element {
protected:
	sdle::Texture texture_start;
	sdle::Texture texture_mid;
	sdle::Texture texture_end;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);

	int m_value, m_maxvalue;

public:
	ProgressBar(Element* parent);
	ProgressBar(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end);
	ProgressBar(Element* parent, const TiXmlElement*, CacheManager&);
	ProgressBar(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager&);

	virtual void Draw(unsigned int delay = 0);

	void Click();

	void setValue(int value);
	void setMaxValue(int maxvalue);
	int getValue() const;
	int getMaxValue() const;
};

}

#endif /* __GUI_PROGRESSBAR_H */
