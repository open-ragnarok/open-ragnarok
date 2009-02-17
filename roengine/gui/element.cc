/* $Id$ */
#include "stdafx.h"

#include "element.h"
#include "gui.h"

GUI::Element::Element() {
	m_parent = NULL;
	m_visible = true;
	m_focusable = true;
	m_fullscreen = false;
	pos_x = 0;
	pos_y = 0;
}

GUI::Element::Element(Element* parent) {
	m_parent = parent;
	m_focusable = true;
	m_visible = true;
	m_fullscreen = false;
	pos_x = 0;
	pos_y = 0;
}

GUI::Element::~Element() {
	std::vector<Element*>::iterator itr = m_children.begin();
	while(itr != m_children.end()) {
		delete(*itr);
		itr++;
	}
	m_children.clear();
}

void GUI::Element::Draw() {
	if (!m_visible)
		return;

	glPushMatrix();
	glTranslatef((float)pos_x, (float)pos_y, 0);
	Window(0, 0, texture);

	std::vector<Element*>::iterator itr = m_children.begin();
	while (itr != m_children.end()) {
		(*itr)->Draw();
		itr++;
	}

	glPopMatrix();
}

void GUI::Element::Window(float x, float y, const Texture::Pointer& tp) const {
	glEnable(GL_TEXTURE_2D);
	tp.Activate();

	float w, h;
	float u, v;

	if (m_fullscreen) {
		Gui* gui = Gui::getSingletonPtr();
		w = (float)gui->getWidth();
		h = (float)gui->getHeight();
	}
	else if (!tp.isValid()) {
		w = (float)this->w;
		h = (float)this->h;
	}
	else {
		w = (float)tp->getWidth();
		h = (float)tp->getHeight();
	}

	if (tp.isValid()) {
		u = tp->getMaxU();
		v = tp->getMaxV();
	}
	else {
		u = 0;
		v = 0;
	}

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, v);
	glVertex3f(x,   y,   0);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x,   y+h, 0);

	glTexCoord2f(u, 0.0f);
	glVertex3f(x+w, y+h, 0);

	glTexCoord2f(u, v);
	glVertex3f(x+w, y,   0);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void GUI::Element::add(Element* e) {
	m_children.push_back(e);
}

void GUI::Element::setPos(int x, int y) {
	pos_x = x;
	pos_y = y;
}

void GUI::Element::setSize(int w, int h) {
	this->w = w;
	this->h = h;
}

GUI::Element* GUI::Element::loadXml(const std::string& name, TextureManager& tm, FileManager& fm) {
	TiXmlDocument doc;

	if (!doc.LoadFile(name)) {
		std::cerr << "Error loading file " << name << std::endl;
		return(NULL);
	}

	TiXmlElement* node;
	node = doc.FirstChildElement();

	return(loadXml(NULL, node, tm, fm));
}

GUI::Element::Element(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) {
	const TiXmlAttribute* attr = node->FirstAttribute();
	std::string attrname;

	m_parent = parent;
	m_visible = true;
	m_focusable = true;
	m_fullscreen = false;
	pos_x = 0;
	pos_y = 0;
	w = 0;
	h = 0;

	while (attr != NULL) {
		attrname = attr->Name();

		if (attrname == "x") {
			if (attr->QueryIntValue(&pos_x) != TIXML_SUCCESS) {
				printf("ERROR");
			}
		}
		else if (attrname == "y") {
			if (attr->QueryIntValue(&pos_y) != TIXML_SUCCESS) {
				printf("ERROR");
			}
		}
		else if (attrname == "width") {
			if (attr->QueryIntValue(&w) != TIXML_SUCCESS) {
				printf("ERROR");
			}
		}
		else if (attrname == "height") {
			if (attr->QueryIntValue(&h) != TIXML_SUCCESS) {
				printf("ERROR");
			}
		}
		else if (attrname == "name") {
			name = attr->Value();
		}
		else if (attrname == "texture") {
			std::string tn = attr->Value();
			if (tn[0] != '\\') {
				std::string aux = "texture\\";
				aux += RO::EUC::user_interface;
				aux += "\\";
				tn = aux + tn;
			}
			else {
				tn = tn.substr(1);
			}

			texture = tm.Register(fm, tn);
		}

		attr = attr->Next();
	}

	if (w == 0 && texture.isValid())
		w = texture->getWidth();
	if (h == 0 && texture.isValid())
		h = texture->getHeight();

	if (pos_x == -1)
		CenterX();
	if (pos_y == -1)
		CenterY();
}

int GUI::Element::getW() const {
	if (!m_fullscreen) {
		if (w != 0)
			return(w);
		else
			if (texture.isValid())
				return(texture->getWidth());
		return(0);
	}
	return(Gui::getSingleton().getWidth());
}

int GUI::Element::getH() const {
	if (!m_fullscreen) {
		if (h != 0)
			return(h);
		else
			if (texture.isValid())
				return(texture->getHeight());
		return(0);
	}
	return(Gui::getSingleton().getHeight());
}

void GUI::Element::CenterX() {
	int pw;
	if (m_parent == NULL)
		pw = Gui::getSingleton().getWidth();
	else
		pw = m_parent->getW();

	pos_x = (pw - getW()) / 2;
}

void GUI::Element::CenterY() {
	int ph;
	if (m_parent == NULL)
		ph = Gui::getSingleton().getHeight();
	else
		ph = m_parent->getH();

	pos_y = (ph - getH()) / 2;
}


GUI::Element* GUI::Element::loadXml(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) {
	std::string nodetype = node->Value();
	GUI::Element* ret = NULL;

	if (nodetype == "window") {
		ret = new GUI::Window(parent, node, tm, fm);
	}
	else if(nodetype == "textarea") {
		ret = new GUI::Window(parent, node, tm, fm);
	}
	else if(nodetype == "button") {
		ret = new GUI::Window(parent, node, tm, fm);
	}
	else {
		// Default
		ret = new Element(parent, node, tm, fm);
	}

	if (nodetype == "desktop")
		ret->setFullscreen(true);

	if (ret == NULL)
		return(NULL);

	const TiXmlElement* child = node->FirstChildElement();
	GUI::Element* aux;
	while (child != NULL) {
		aux = GUI::Element::loadXml(ret, child, tm, fm);
		if (aux != NULL)
			ret->add(aux);
		child = child->NextSiblingElement();
	}

	return(ret);
}


void GUI::Element::setTexture(const Texture::Pointer& tp) {
	texture = tp;
}

void GUI::Element::setFullscreen(bool fs) {
	m_fullscreen = fs;
}
