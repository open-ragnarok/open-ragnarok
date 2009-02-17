#ifndef __GUI_TEXTINPUT_H
#define __GUI_TEXTINPUT_H

#include "element.h"

namespace GUI {
	class TextInput : public Element {
	protected:
		std::string m_text;
	public:
		void setText(const std::string&);
		std::string& getText();
		const std::string& getText() const;
		virtual void Draw();
	};
}

#endif /* __GUI_TEXTINPUT_H */