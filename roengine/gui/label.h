/* $Id$ */
#ifndef __GUI_LABEL_H
#define __GUI_LABEL_H

#include "element.h"

namespace GUI {
	class Label : public Element {
	protected:
		std::string m_text;

		virtual bool ParseXmlAttr(const TiXmlAttribute*, TextureManager&, FileManager&);

	public:
		Label(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		virtual void Draw(unsigned int delay = 0);

		Label& setText(const std::string&);
		std::string& getText();
		const std::string& getText() const;
	};
}

#endif /* __GUI_LABEL_H */