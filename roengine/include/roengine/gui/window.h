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
	Window(Element* parent);
	Window(const sdle::Texture&, Element* parent = NULL);
	Window(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);

	/**
	 * Creates an window with a given parent, loads the background parameter and sets the element size to the
	 * texture size.
	 * If the texture cannot be loaded, this function behaves like the constructor Window(Element* parent);
     *
	 * @param parent the Parent element
     * @param background the background name
	 * @param tm
	 * @param fm
	 */
	Window(Element* parent, const std::string& background, TextureManager& tm, FileManager& fm);

	virtual ~Window();

	void setCaption(const std::string&);
	virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseRelease(int x, int y, int button);
	virtual bool HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy);
};

}

#endif /* __GUI_WINDOW_H */
