/* $Id$ */
#ifndef __GUI_WINDOW_H
#define __GUI_WINDOW_H

#include "element.h"

namespace GUI {

class Window : public Element {
protected:
	std::string caption;
public:
	bool IsMouseDowning;
	Window();
	virtual ~Window();
	Window(const rogl::Texture::Pointer&);
	Window(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

	void setCaption(const std::string&);
	virtual bool HandleKeyDown(int key, int mod = 0);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseRelease(int x, int y, int button);
	virtual bool HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy);
};

}

#endif /* __GUI_WINDOW_H */
