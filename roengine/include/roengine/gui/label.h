/* $Id$ */
#ifndef __GUI_LABEL_H
#define __GUI_LABEL_H

#include "element.h"
#include <string>

namespace GUI {
	class Label : public Element {
	private:
		bool m_shadow;
		bool m_rightalign;
	protected:
		std::string m_text;

		virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);

	public:
		Label(Element* parent, const TiXmlElement*, CacheManager&);

		virtual void Draw(unsigned int delay = 0);

		Label& setText(const std::string&);
		Label& setText(const char* s);
		std::string& getText();
		const std::string& getText() const;
	};
}

#endif /* __GUI_LABEL_H */
