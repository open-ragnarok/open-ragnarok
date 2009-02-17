/* $Id$ */
#ifndef __GUI_H
#define __GUI_H

#include "singleton.h"

#include "window.h"
#include "textinput.h"
#include "../base_cache.h"
#include "font.h"

// Font handlers
#include "glf_font.h"

#include <string>
#include <vector>

namespace GUI {

class Gui : public Singleton<Gui> {
protected:
	Element* m_desktop;
	Element* active;
	int m_width;
	int m_height;

	BaseCache<Font> m_fonts;

	const Font* m_defaultFont;

public:
	Gui();
	~Gui();

	/** Intializes GUI, loads default fonts and register textures */
	void Init(int, int);

	void setSize(int, int);

	Element* getDesktop();
	void setDesktop(Element*);

	void Draw();

	void InjectMousePos(int, int);
	void InjectMouseClick(int, int modifier = 0);
	void InjectKeyPress(int);
	void InjectKeyRelease(int);

	BaseCache<Font>& FontManager();
	const BaseCache<Font>& FontManager() const;
	const Font* getDefaultFont();

	int getWidth() const;
	int getHeight() const;
};

}

#endif /* __GUI_H */