/* $Id$ */
#ifndef __GUI_TEXTINPUT_H
#define __GUI_TEXTINPUT_H

#include "label.h"

namespace GUI {
	class TextInput : public Label {
	protected:
		unsigned int m_pos;
	public:
		TextInput(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

		virtual bool HandleKeyDown(int key, int mod = 0);
	};
}

#endif /* __GUI_TEXTINPUT_H */