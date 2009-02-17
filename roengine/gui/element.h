/* $Id$ */
#ifndef __GUI_ELEMENT_H
#define __GUI_ELEMENT_H

#include <vector>

#include "../texture.h"
#include "../texturemanager.h"

#include "tinyxml/tinyxml.h"

namespace GUI {

class Element {
protected:
	Element* m_parent;
	std::string name;
	bool m_visible;
	bool m_focusable;
	bool m_fullscreen;

	std::vector<Element*> m_children;
	Texture::Pointer texture;

	int pos_x, pos_y;
	int w, h;

	void Window(float x, float y, const Texture::Pointer& tp) const;
public:
	Element();
	Element(Element* parent);
	Element(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);
	virtual ~Element();

	void setTexture(const Texture::Pointer&);

	virtual void Draw();
	void add(Element*);
	void setPos(int, int);
	void setSize(int, int);
	void setFullscreen(bool = false);

	void CenterX();
	void CenterY();

	int getW() const;
	int getH() const;

	static Element* loadXml(const std::string&, TextureManager&, FileManager&);
	static Element* loadXml(Element* parent, const TiXmlElement* node, TextureManager&, FileManager&);
};

}

#endif
