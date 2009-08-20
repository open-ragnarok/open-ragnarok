/* $Id$ */
#ifndef __GUI_LIST_H
#define __GUI_LIST_H

#include "element.h"

namespace GUI {
	class List : public Element {
	protected:
		std::vector<std::string> m_items;
		int selected;

		/** First element drawn on the screen.
		 * Specially useful if we have a long list and not all items are visible at once.
		 */
		int m_first;
		bool actived;

		// virtual bool ParseXmlAttr(const TiXmlAttribute*, TextureManager&, FileManager&);
	public:
		List(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		virtual void Draw(unsigned int delay = 0);
		virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
		virtual bool HandleMouseDown(int x, int y, int button);

		virtual void onGetFocus();
		virtual void onLoseFocus();

		int getSelected() const;
		void setSelected(const int&);
		void add(const std::string&);
		void clear();
	};
}

#endif /* __GUI_LIST_H */
