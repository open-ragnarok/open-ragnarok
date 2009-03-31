/* $Id$ */
#ifndef __GUI_TEXTINPUT_H
#define __GUI_TEXTINPUT_H

#include "label.h"

namespace GUI {
	class TextInput : public Label {
	protected:
		static unsigned int cursor_delay;

		unsigned int m_pos;
		unsigned int m_start;
		unsigned int m_delay;
		bool m_bar;
	public:
		TextInput(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		virtual void Draw(unsigned int delay = 0);
		virtual bool HandleKeyDown(int key, int mod = 0);
	};
}

#endif /* __GUI_TEXTINPUT_H */