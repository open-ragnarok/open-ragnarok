/* $Id: checkbox.h $ */
#ifndef __GUI_CHECKBOX_H
#define __GUI_CHECKBOX_H

#include "button.h"

namespace GUI {

class CheckBox : public Button {
protected:
	sdle::Texture texture_base;
	sdle::Texture texture_checked;

	bool m_checked;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);
public:
	CheckBox(Element* parent);
	CheckBox(Element* parent, const sdle::Texture& tex_base, const sdle::Texture& tex_checked);
	CheckBox(Element* parent, const TiXmlElement*, CacheManager&);
	CheckBox(Element* parent, const std::string& base, const std::string& checked, CacheManager&);

	virtual void Draw(unsigned int delay = 0);

	void Click();

	void setChecked(bool checked);
	bool isChecked() const;
};

}

#endif /* __GUI_CHECKBOX_H */
