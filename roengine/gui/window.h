/* $Id$ */
#ifndef __GUI_WINDOW_H
#define __GUI_WINDOW_H

#include "element.h"

namespace GUI {

class Window : public Element {
protected:
	std::string caption;
public:
	Window();
	virtual ~Window();
	Window(const Texture::Pointer&);
	Window(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

	void setCaption(const std::string&);
	virtual bool HandleKeyDown(int key, int mod = 0);
};

}

#endif /* __GUI_WINDOW_H */