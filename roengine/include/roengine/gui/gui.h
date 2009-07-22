/* $Id$ */
#ifndef __GUI_H
#define __GUI_H

// Objects
#include "desktop.h"
#include "window.h"
#include "textinput.h"
#include "button.h"
#include "label.h"
#include "textinput.h"
#include "list.h"

// Other stuff
#include "singleton.h"
#include "../base_cache.h"
#include "../vector.h"
#include "font.h"
#include "event.h"

// Font handlers
#include "glf_font.h"

#include <string>
#include <vector>

/**
 * \namespace GUI Namespace for the Graphic User Interface.
 * Intending to mimic the Original Ragnarok user interface and go beyond.
 */
namespace GUI {

/**
 * Singleton class for handling all GUI needs.
 * Most of these objects mimics the original Ragnarok Online user interface.
 */
class Gui : public Singleton<Gui> {
protected:
	Desktop* m_desktop;
	Element* active;
	int m_width;
	int m_height;

	BaseCache<Font> m_fonts;
	std::vector<Event> m_events;

	const Font* m_defaultFont;

public:
	Gui();
	~Gui();

	/** Intializes GUI, loads default fonts and register textures */
	void Init(int, int);

	/** Sets the size of the desktop. This is used (mostly) by fullscreen and desktop objects */
	void setSize(int, int);

	/** Clears the GUI and all the elements */
	void clear();

	Desktop* getDesktop();
	const Desktop* getDesktop() const;
	Element* getActiveElement();
	void setDesktop(Desktop*);
	void setDesktop(const std::string&);
	void setFocus(Element*);

	void Draw(unsigned int delay = 0, Vector3f CameraLook = Vector3f::UNIT_Z);

	void InjectMousePos(int, int);
	bool InjectMouseClick(int, int, int buttons = 0, int modifier = 0);
	bool InjectKeyPress(const int&, const int& mod = 0);
	bool InjectKeyRelease(const int&, const int& mod = 0);

	BaseCache<Font>& FontManager();
	const BaseCache<Font>& FontManager() const;
	const Font* getDefaultFont();

	int getWidth() const;
	int getHeight() const;

	void PushEvent(const Event&);
	Event PopEvent();
	void ProcessEvents();
	int GetEventCount() const;

	/** Finds an object of a given name. No two objects can have the same name. */
	Element* operator[] (const std::string&);

	/** Finds an object of a given name. No two objects can have the same name. */
	const Element* operator[] (const std::string&) const;
};

}

#endif /* __GUI_H */
