/* $Id$ */
#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "element.h"

namespace GUI {
	class Button : public Element {
	protected:
		rogl::Texture::Pointer texture_base;
		rogl::Texture::Pointer texture_active;
		rogl::Texture::Pointer texture_hover;
		rogl::Texture::Pointer texture_disabled;

		virtual bool ParseXmlAttr(const TiXmlAttribute*, TextureManager&, FileManager&);

	public:
		Button(Element* parent);
		Button(Element* parent, const rogl::Texture::Pointer& base);
		Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active);
		Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active, const rogl::Texture::Pointer& hover);
		Button(Element* parent, const rogl::Texture::Pointer& base, const rogl::Texture::Pointer& active, const rogl::Texture::Pointer& hover, const rogl::Texture::Pointer& disabled);

		Button(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		/**
		 * Creates an window with a given parent, loads the background parameter and sets the element size to the
		 * texture size.
		 * If the texture cannot be loaded, this function behaves like the constructor Button(Element* parent);
		 *
		 * @param parent the Parent element
		 * @param background the background name
		 * @param tm
		 * @param fm
		 */
		Button(Element* parent, const std::string& background, TextureManager& tm, FileManager& fm);

		virtual void Draw(unsigned int delay = 0);

		void Click();

		virtual bool HandleMouseDown(int x, int y, int button);
		virtual bool HandleKeyDown(int key, int mod = 0);
		virtual void onGetFocus();
		virtual void onLoseFocus();
	};
}

#endif /* __GUI_BUTTON_H */
