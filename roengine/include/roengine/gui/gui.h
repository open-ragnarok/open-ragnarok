/* $Id$ */
#ifndef __GUI_H
#define __GUI_H

// Objects
#include "desktop.h"
#include "window.h"
#include "textinput.h"
#include "button.h"
#include "label.h"
#include "list.h"
#include "dialog.h"
//#include "progressbar.h"
//#include "chatwindow.h"
#include "buttonbar.h"
#include "dropdownlist.h"
#include "scrollbar.h"
#include "textbox.h"
#include "checkbox.h"
#include "sysbox.h"
#include "hpspbar.h"
#include "chatlog.h"
#include "inventorywindow.h"
#include "slider.h"

// Other stuff
#include "singleton.h"
#include "../base_cache.h"
#include "../vector.h"
#include "event.h"

#include "../cache_manager.h"

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
	/** The current active Desktop */
	Desktop* m_desktop;
	Desktop* m_inactiveDesktop;

	/** The current active Element */
	Element* active;

	/** GUI width */
	int m_width;

	/** GUI height */
	int m_height;

	/** Stores the registered fonts. These are all destroyed when this object is destroyed. */
	BaseCache<sdle::Font> m_fonts;
	std::vector<Event> m_events;

	/** 
	 * The default font used by GUI Components.
	 * If not changed, it's always the first loaded font.
	 * Objects not explicitally using a custom font will use this one. Changing this font will
	 * change the entire user interface font.
	 */
	const sdle::Font* m_defaultFont;

	/** Background for the dialog box */
	std::string m_msgbox_bg;

	/** Background for the dialog box ok button */
	std::string m_msgbox_btnok;

	/** Background for the dialog box cancel button */
	std::string m_msgbox_btncancel;

	/** Background for the dialog box exit button */
	std::string m_msgbox_btnexit;

public:
	Gui();
	~Gui();

	std::string getMsgboxBg();
	std::string getMsgboxOk();
	std::string getMsgboxCancel();
	std::string getMsgboxExit();

	/** Intializes GUI, loads default fonts and register textures */
	void Init(int, int);

	/** Sets the size of the desktop. This is used (mostly) by fullscreen and desktop objects */
	void setSize(int, int);

	/** Clears the GUI and all the elements */
	void clear();

	/**
	 * Returns the active desktop object
	 * @returns the active desktop or NULL if no desktop is active.
	 */
	Desktop* getDesktop();

	/**
	 * Returns the active desktop object
	 * @returns the active desktop or NULL if no desktop is active.
	 */
	const Desktop* getDesktop() const;
	Element* getActiveElement();
	Element* getElementMousePos(int, int);

	/**
	 * Sets the active desktop to the given object.
	 * @param desktop the Desktop object
	 */
	void setDesktop(Desktop* desktop);

	/**
	 * Sets the active desktop to the object with the given name.
	 * To set the desktop to "no desktop", pass an empty string with the name.
	 * 
	 * @param name string with the name of the object to be used as desktop.
	 * @returns true if the name was successfully found and setted. false if object not found.
	 */
	bool setDesktop(const std::string& name);

	/**
	 * Sets the focus to the given element.
	 */
	void setFocus(Element*);

	/**
	 * Draws the graphic user interface
	 * @param delay
	 * @param cameraLook
	 */
	void Draw(unsigned int delay = 0, Vector3f CameraLook = Vector3f::UNIT_Z);

	/**
	 * Injects the mouse position on the GUI. Useful to change color on hovered buttons,
	 * change mouse pointers and other useful features.
	 *
	 * @param x x coordinate relative to the top left corner of the window
	 * @param y y coordinate relative to the top left corner of the window
	 */
	void InjectMousePos(int x, int y);

	/**
	 * Injects an MouseClick event to the GUI. This feature is important to handle button/api clicks
	 * on the objects.
	 *
	 * @param x x coordinate relative to the top left corner of the window
	 * @param y y coordinate relative to the top left corner of the window
	 * @param buttons
	 * @param modifier
	 * @returns boolean true when the click was handled, false otherwise (like, user clicked on nothing)
	 */
	bool InjectMouseClick(int x, int y, int buttons = 0, int modifier = 0);
	bool InjectMouseRelease(int, int, int buttons = 0, int modifier = 0);
	bool InjectKeyPress(SDL_Event*, const int& mod = 0);
	bool InjectKeyRelease(SDL_Event*, const int& mod = 0);
	bool InjectMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	BaseCache<sdle::Font>& FontManager();
	const BaseCache<sdle::Font>& FontManager() const;
	const sdle::Font* getDefaultFont();

	/** Retrieves current width */
	int getWidth() const;
	
	/** Retrieves current height */
	int getHeight() const;

	void PushEvent(const Event&);
	Event PopEvent();
	void ProcessEvents();
	int GetEventCount() const;

	/**
	 * Outputs a text using the default font
	 * @param text string with the text to be drawn
	 * @param x x coordinate position
	 * @param y y coordinate position
	 * @param z z coordinate position
	 * @param MaxLen Maximum number of characters to be drawn
	 */
	void textOut(const std::string& text, float x, float y, float z, int MaxLen);
	//int TextOutEx(IN TTF_Font *Font, IN const LCHAR *Text, IN float x, IN float y,IN float z, IN Uint32 FColor, IN Uint32 BColor, IN Uint8 Alpha);
	int TextOutEx(TextEditor *G_Text);
	void ClearFont_Screen(TextEditor *G_Text);
	int calcTextWidth(const char* text) const;

	/**
	 * Finds an object of a given name. No two objects can have the same name.
	 * @param obj Name of the object
	 * @returns An pointer to the Element or NULL in case of element not found.
	 */
	Element* operator[] (const std::string& obj);

	/**
	 * Finds an object of a given name. No two objects can have the same name.
	 * @param obj Name of the object
	 * @returns An pointer to the Element or NULL in case of element not found.
	 */
	const Element* operator[] (const std::string& obj) const;

	void Dialog(const std::string& title, const std::string& text, CacheManager& cache, int buttons = BTN_OK | BTN_CANCEL);

	void setDefaultFont(const sdle::Font*);
};

}

#endif /* __GUI_H */
