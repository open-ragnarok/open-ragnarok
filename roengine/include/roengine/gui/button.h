/* $Id$ */
#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "element.h"

namespace GUI {
	class Button : public Element {
	protected:
		sdle::Texture texture_base;
		sdle::Texture texture_hover;
		sdle::Texture texture_click;
		sdle::Texture texture_disabled;

		virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);

	public:
		Button(Element* parent);
		Button(Element* parent, const sdle::Texture& base);
		Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active);
		Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active, const sdle::Texture& hover);
		Button(Element* parent, const sdle::Texture& base, const sdle::Texture& active, const sdle::Texture& hover, const sdle::Texture& disabled);

		Button(Element* parent, const TiXmlElement*, CacheManager&);

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
		Button(Element* parent, const std::string& background, CacheManager&);

		virtual Type getType() { return typeButton; }

		virtual void Draw(unsigned int delay = 0);

		void Down();
		void Click();

		virtual bool HandleMouseDown(int x, int y, int button);
		virtual bool HandleMouseRelease(int x, int y, int button);
	//	virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
		virtual void onGetFocus();
		virtual void onLoseFocus();
	};
}

#endif /* __GUI_BUTTON_H */
