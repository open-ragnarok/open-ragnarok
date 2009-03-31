/* $Id$ */
#ifndef __GUI_ELEMENT_H
#define __GUI_ELEMENT_H

#include <vector>

#include "rogl/texture.h"
#include "../texturemanager.h"

#include "tinyxml/tinyxml.h"

namespace GUI {

class Element;

/**
 * Checks if the point (x, y) is inside a given element.
 * @param e Const Element*
 * @param x int X coordinate
 * @param y int Y coordinate
 */
bool isInside(const Element* e, int x, int y);

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

	static GUI::Element::Cache m_elements;
	static std::string createName();

	Element* m_parent;
	/** Indicates if this object is visible or not. If it's false, not the object nor its children will be drawn */
	bool m_visible;

	/** Indicates if this object is transparent or not. If it's false, the object will not be drawn, but its children will */
	bool m_transparent;

	bool m_focusable;
	bool m_fullscreen;
	bool m_enabled;

	std::vector<Element*> m_children;
	Element* m_active_child;
	rogl::Texture::Pointer texture;

	int pos_x, pos_y;
	int w, h;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, TextureManager&, FileManager&);
	void ParseFromXml(const TiXmlElement*, TextureManager&, FileManager&);

	void Window(float x, float y, const rogl::Texture::Pointer& tp) const;
public:
	Element();
	Element(Element* parent);
	Element(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);
	virtual ~Element();

	void setTexture(const rogl::Texture::Pointer&);

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
	void setVisible(bool = true);
	void setTransparent(bool = false);
	void setEnabled(bool = true);
	void setActive();
	bool setName(const std::string&);

	bool isVisible() const;
	bool isTransparent() const;
	bool isEnabled() const;

	const std::string& getName();

	void CenterX();
	void CenterY();

	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;

	bool isFocusable() const;

	/* Events */
	virtual bool HandleKeyDown(int key, int mod = 0);
	virtual bool HandleKeyUp(int key, int mod = 0);
	virtual bool HandleMouseMove(int x, int y);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseUp(int x, int y, int button);

	virtual void onGetFocus();
	virtual void onLoseFocus();

	void setActiveChild(Element*);
	Element* getActiveChild();
	const Element* getActiveChild() const;

	static Element* loadXml(Element* parent, const TiXmlElement* node, TextureManager&, FileManager&);

	static Element* getElement(const std::string&);
	static GUI::Element::Cache& getCache();
};

}

#endif

