/* $Id: hpspbar.h $ */
#ifndef __GUI_HPSPBAR_H
#define __GUI_HPSPBAR_H

#include "element.h"

namespace GUI {

/**
 * An implementation for a progress bar (used, for example, in the HP and SP bars)
 */
class HpSpBar : public Element {
protected:
	sdle::Texture texture_start;
	sdle::Texture texture_mid;
	sdle::Texture texture_end;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);

	int m_value, m_maxvalue;

public:
	HpSpBar(Element* parent);
	HpSpBar(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end);
	HpSpBar(Element* parent, const TiXmlElement*, CacheManager&);
	HpSpBar(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager&);

	virtual void Draw(unsigned int delay = 0);

	void Click();

	void setValue(int value);
	void setMaxValue(int maxvalue);
	int getValue() const;
	int getMaxValue() const;
};

}

#endif /* __GUI_HPSPBAR_H */
