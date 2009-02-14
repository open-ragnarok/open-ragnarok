#ifndef __GUI_FONT_H
#define __GUI_FONT_H

#include <string>

namespace GUI {
	class Font {
	protected:
		virtual void textOut (const std::string&, float x, float y, float z) const = 0;
	};
}

#endif /* __GUI_FONT_H */