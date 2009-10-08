/* $Id$ */
#ifndef __GUI_ELEMENT_H
#define __GUI_ELEMENT_H

#include <vector>

#include "sdle/texture.h"
#include "../cache_manager.h"

#include "tinyxml/tinyxml.h"

#define  SDL_OPACITY            255

#define  SDL_BLACK              0
#define  SDL_WHITE              16777215
#define  SDL_TURQUOISE          65535
#define  SDL_BLUE               255
#define  SDL_PINK               16711935
#define  SDL_RED                16711680
#define  SDL_YELLOW             16776960
#define  SDL_GREEN              65280
#define  SDL_SILVER             12632256
#define  SDL_BLOWN              10040064
#define  SDL_MAROON             8388608
#define  SDL_GRAY               8421504
#define  SDL_ROSE               16751052
#define  SDL_ORANGE             16737792
#define  SDL_SHALLOW_YELLOW     16777113
#define  SDL_GOLD               16763904
#define  SDL_TAWNY              16764057
#define  SDL_OLIVE              3355392
#define  SDL_DARK_YELLOW        8421376
#define  SDL_SEA_GREEN          3381606
#define  SDL_BOTTLE_GREEN       13056
#define  SDL_SKY_BLUE           52479
#define  SDL_BABY_BLUE          10079487
#define  SDL_VIOLET             8388766
#define  SDL_LAVENDER           13408767
#define  SDL_LIME               10079232

#ifdef UNICODE
        typedef  unsigned short LCHAR;
#else
        typedef  char LCHAR;
#endif

#ifdef UNICODE
        #define  AU(str)  L##str   
#else
        #define  AU(str)  str      
#endif

namespace GUI {

class Element;

/**
 * Basic class for building the interface. All GUI objects must come from this one.
 * This class can still be used for basic display. No events will be caught on this class.
 */
class Element {
private:
	std::string name;

protected:
	class Cache : public BaseCache<Element> {
	public:
		bool add(Element*);
	};

	/**
	 * Cache contained all elements created.
	 */
	static GUI::Element::Cache m_elements;

	/**
	 * Generates an unique element name when no name is given.
	 */
	static std::string createName();

	/** Pointer to the parent element (if any) */
	Element* m_parent;
	
	/** Indicates if this object is visible or not. If it's false, not the object nor its children will be drawn */
	bool m_visible;

	/** Indicates if this object is transparent or not. If it's false, the object will not be drawn, but its children will */
	bool m_transparent;

	/** Indicates if this object is semi-transparent or not. If it's true, the object will be drawn 50% trensparent, with it's childen too */
	bool m_stransparent;

	bool m_focusable;
	bool m_fullscreen;
	bool m_enabled;
	bool m_MouseIn;

	std::vector<Element*> m_children;
	Element* m_active_child;
	sdle::Texture texture;

	int pos_x, pos_y, MaxLen;
	int w, h;

	/** 
	 * Move Area
	 * These coordinates define a place where you can click and the corresponding window will move
	 */
	int mw, mh;
	int mx, my;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);
	void ParseFromXml(const TiXmlElement*, CacheManager&);

	void Window(float x, float y, const sdle::Texture& tp);
	void Window(float x, float y, float w, float h, const sdle::Texture& tp);
	
	/** Draws a sequence of 3 textures.
	 * The first and last are only drawn once, the mid one is repeated until the width gap is fixed.
	 *
	 * @param x
	 * @param y
	 * @param w
	 * @param h Height to draw. Currently ignored.
	 * @param start
	 * @param mid
	 * @param end
	 */
	void WindowSeq(float x, float y, float w, float h, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end);

public:
	Element();
	float m_opacity;
	/**
	 * Creates an empty element with a parent
	 * @param parent the Parent element
	 */
	Element(Element* parent);
	Element(Element* parent, const TiXmlElement*, CacheManager&);
	
	/**
	 * Creates an element with a given parent, loads the background parameter and sets the element size to the
	 * texture size.
	 * If the texture cannot be loaded, this function behaves like the constructor Element(Element* parent);
     *
	 * @param parent the Parent element
     * @param background the background name
	 * @param tm
	 * @param fm
	 */
	Element(Element* parent, const std::string& background, CacheManager&);
	virtual ~Element();

	void setTexture(const sdle::Texture&);
	void SetMouseInFlag(bool flag);

	void Render(unsigned int delay = 0);
	virtual void Draw(unsigned int delay = 0);
	virtual void beforeDraw(unsigned int delay = 0);
	virtual void afterDraw(unsigned int delay = 0);

	/** Adds a child element to this one */
	void add(Element*);

	/** Deletes a element from the children list. This removes only a first child. */
	void del(const Element*);

	/** Sets the element position */
	void setPos(const int&, const int&);

	/** Sets the element size. If the element size is not set manually, it will use the texture size. */
	void setSize(const int&, const int&);
	void setFullscreen(bool = false);
	virtual void setVisible(bool = true);
	void setTransparent(bool = false);
	void setStransparent(bool = false);
	void setEnabled(bool = true);
	void setActive();
	bool setName(const std::string&);

	bool isVisible() const;
	bool isTransparent() const;
	bool isStransparent() const;
	bool isEnabled() const;

	const std::string& getName();

	void CenterX();
	void CenterY();

	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;
	int getMW() const;
	int getMH() const;

	bool isFocusable() const;

	/* Events */
	virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleKeyUp(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleMouseMove(int x, int y);
	virtual bool HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseRelease(int x, int y, int button);

	virtual void onGetFocus();
	virtual void onLoseFocus();

	void setActiveChild(Element*);
	Element* getActiveChild();
	const Element* getActiveChild() const;

	/* Static stuff */
	static Element* loadXml(Element* parent, const TiXmlElement* node, CacheManager&);

	/** Reteieves an element based on its name */
	static Element* getElement(const std::string&);
	static GUI::Element::Cache& getCache();

	/**
	 * Checks if the point (x, y) is inside a given element.
	 * @param e Const Element*
	 * @param x int X coordinate
	 * @param y int Y coordinate
	 */
	static bool isInside(const Element* e, int x, int y);
	static bool isInsideMoveArea(const GUI::Element* e, int x, int y);

	static sdle::Texture LoadTexture(std::string, CacheManager&);
};

}

#endif

