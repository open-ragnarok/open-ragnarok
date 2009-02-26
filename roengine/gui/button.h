/* $Id$ */
#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "element.h"

namespace GUI {
	class Button : public Element {
	protected:
		Texture::Pointer texture_base;
		Texture::Pointer texture_active;
		Texture::Pointer texture_hover;
		Texture::Pointer texture_disabled;

		virtual bool ParseXmlAttr(const TiXmlAttribute*, TextureManager&, FileManager&);

	public:
		Button(Element* parent, const Texture::Pointer& base);
		Button(Element* parent, const Texture::Pointer& base, const Texture::Pointer& active);
		Button(Element* parent, const Texture::Pointer& base, const Texture::Pointer& active, const Texture::Pointer& hover);
		Button(Element* parent, const Texture::Pointer& base, const Texture::Pointer& active, const Texture::Pointer& hover, const Texture::Pointer& disabled);

		Button(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		virtual void Draw();

		virtual bool HandleMouseDown(int x, int y, int button);
	};
}

#endif /* __GUI_BUTTON_H */