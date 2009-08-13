/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/element.h"
#include "roengine/gui/gui.h"

#include <GL/gl.h>

GUI::Element::Cache GUI::Element::m_elements;

bool GUI::isInside(const GUI::Element* e, int x, int y) {
	if ((x >= e->getX()) && (y >= e->getY()) && (x <= (e->getX() + e->getW())) && (y <= (e->getY() + e->getH())))
		return(true);
	return(false);
}

bool GUI::isInsideMoveArea(const GUI::Element* e, int x, int y) {
	if ((x >= e->getX()) && (y >= e->getY()) && (x <= (e->getX() + e->getMW())) && (y <= (e->getY() + e->getMH())))
		return(true);
	return(false);
}

bool GUI::Element::Cache::add(GUI::Element* e) {
	if (e->getName() == "")
		e->setName(GUI::Element::createName());

	return(BaseCache<Element>::add(e->getName(), e));
}

std::string GUI::Element::createName() {
	std::string base_name = "NoName";
	int i = 0;
	char buf[256];
	sprintf(buf, "%s-%d", base_name.c_str(), i);
	
	while (m_elements.exists(buf)) {
		i++;
		sprintf(buf, "%s-%d", base_name.c_str(), i);
	}

	return(buf);
}

GUI::Element::Element() {
	m_parent = NULL;
	m_focusable = true;
	m_transparent = false;
	m_visible = true;
	m_fullscreen = false;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;

	m_elements.add(this);
}

GUI::Element::Element(Element* parent) {
	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_visible = true;
	m_fullscreen = false;
	m_enabled = true;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	MaxLen = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;

	m_elements.add(this);
}

GUI::Element::Element(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) {
	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_visible = true;
	m_fullscreen = false;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;

	m_elements.add(this);

	if (node != NULL)
		ParseFromXml(node, tm, fm);
}

GUI::Element::Element(Element* parent, const std::string& background, TextureManager& tm, FileManager& fm) {
	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_visible = true;
	m_fullscreen = false;
	m_enabled = true;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	MaxLen = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;

	m_elements.add(this);

	rogl::Texture::Pointer ptr = tm.Register(fm, background);
	if (ptr.isValid()) {
		texture = ptr;
		setSize(texture->getWidth(), texture->getHeight());
	}
}

GUI::Element::Cache& GUI::Element::getCache() {
	return(m_elements);
}

GUI::Element::~Element() {
	std::vector<Element*>::iterator itr = m_children.begin();
	m_elements.remove(this->name, false);
	while(itr != m_children.end()) {
		Element* e = *itr;
		m_children.erase(itr);
		delete(e);
		itr = m_children.begin();
	}
	m_children.clear();
	if (m_parent != NULL) {
		m_parent->del(this);
		m_parent = NULL;
	}

	//std::cout << "\tElement " << this->name << " successfuly deleted." << std::endl;
	// m_elements.remove(this->name, false);
}

bool GUI::Element::setName(const std::string& n) {
	if (name == n)
		return(true);

	if (name == "") {
		name = n;
		return(true);
	}

	if (!m_elements.rename(name, n))
		return(false);

	name = n;

	return(true);
}

const std::string& GUI::Element::getName() {
	return(name);
}

void GUI::Element::beforeDraw(unsigned int delay) {}
void GUI::Element::afterDraw(unsigned int delay) {}

void GUI::Element::Draw(unsigned int delay) {
	if (!m_visible)
		return;

	glPushMatrix();
	glTranslatef((float)pos_x, (float)pos_y, 0);
	beforeDraw(delay);

	if (!m_transparent)
		Window(0, 0, texture);

	std::vector<Element*>::iterator itr = m_children.begin();
	while (itr != m_children.end()) {
		(*itr)->Draw(delay);
		itr++;
	}

	afterDraw(delay);
	glPopMatrix();
}

void GUI::Element::Window(float x, float y, const rogl::Texture::Pointer& tp) const {
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
}

void GUI::Element::add(Element* e) {
	if (e == NULL)
		return;
	m_children.push_back(e);
	if (m_active_child == NULL) {
		if (e->m_focusable)
		{
			m_active_child = e;

			std::vector<Element*>::iterator itr = m_active_child->m_children.begin();

			if( itr == m_active_child->m_children.end() )
			{
				std::cout << e->name << "::GetFocus" << std::endl;
				e->onGetFocus();
			}
		}
	}
}

void GUI::Element::del(const Element* e) {
	if (e == NULL)
		return;
	std::vector<Element*>::iterator itr = m_children.begin();
	while (itr != m_children.end()) {
		if (*itr == e) {
			m_children.erase(itr);
			return;
		}
		itr++;
	}
}

void GUI::Element::setPos(const int& x, const int& y) {
	pos_x = x;
	pos_y = y;
}

void GUI::Element::setSize(const int& w, const int& h) {
	this->w = w;
	this->h = h;
}

bool GUI::Element::ParseXmlAttr(const TiXmlAttribute* attr, TextureManager& tm, FileManager& fm) {
	std::string attrname = attr->Name();

	if (attrname == "x") {
		if (attr->QueryIntValue(&pos_x) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		if (pos_x <= -2) {
			if (m_parent == NULL)
				pos_x = 0;
			else
				pos_x += m_parent->getW();
		}
		return(true);
	}
	else if (attrname == "y") {
		if (attr->QueryIntValue(&pos_y) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		if (pos_y <= -2) {
			if (m_parent == NULL)
				pos_y = 0;
			else
				pos_y += m_parent->getH();
		}
		return(true);
	}
	else if (attrname == "MaxLen") {
		if (attr->QueryIntValue(&MaxLen) != TIXML_SUCCESS) {
			MaxLen = 0;
		}
		if (MaxLen <= 0) {
			MaxLen = 0;
		}
		return(true);
	}
	else if (attrname == "transparent") {
		std::string value = attr->Value();
		if ((value == "true") || (value == "1"))
			m_transparent = true;
		else
			m_transparent = false;
		return(true);
	}
	else if (attrname == "visible") {
		std::string value = attr->Value();
		if ((value == "false") || (value == "0"))
			m_visible = false;
		else
			m_visible = true;
		return(true);
	}
	else if (attrname == "width" || attrname == "w") {
		if (attr->QueryIntValue(&w) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "height" || attrname == "h") {
		if (attr->QueryIntValue(&h) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "name") {
		if (!setName(attr->Value())) {
			std::cerr << "Error changing name attribute from '" << name << "' to '" << attr->Value() << "'" << std::endl;
		}
		return(true);
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
		return(true);
	}
	else if (attrname == "moveablewidth" || attrname == "mw") {
		if (attr->QueryIntValue(&mw) != TIXML_SUCCESS) {
			printf("ERROR");
			mw = 0;
		}
		return(true);
	}
	else if (attrname == "moveableheight" || attrname == "mh") {
		if (attr->QueryIntValue(&mh) != TIXML_SUCCESS) {
			printf("ERROR");
			mh = 0;
		}
		return(true);
	}

	return(false);
}

void GUI::Element::ParseFromXml(const TiXmlElement* node, TextureManager& tm, FileManager& fm) {
	const TiXmlAttribute* attr = node->FirstAttribute();
	std::string attrname;

	while (attr != NULL) {
		ParseXmlAttr(attr, tm, fm);
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

int GUI::Element::getX() const {
	return(pos_x);
}

int GUI::Element::getY() const {
	return(pos_y);
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

int GUI::Element::getMW() const {
	return(mw);
}

int GUI::Element::getMH() const {
	return(mh);
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
		ret = new GUI::TextInput(parent, node, tm, fm);
	}
	else if(nodetype == "label") {
		ret = new GUI::Label(parent, node, tm, fm);
	}
	else if(nodetype == "button") {
		ret = new GUI::Button(parent, node, tm, fm);
	}
	else if (nodetype == "list") {
		ret = new GUI::List(parent, node, tm, fm);
	}
	else {
		// Default
		ret = new Element(parent, node, tm, fm);
	}

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


void GUI::Element::setTexture(const rogl::Texture::Pointer& tp) {
	texture = tp;
}

void GUI::Element::setFullscreen(bool fs) {
	m_fullscreen = fs;
}

void GUI::Element::setVisible(bool b) {
	m_visible = b;
}

void GUI::Element::setTransparent(bool b) {
	m_transparent = b;
}

void GUI::Element::setEnabled(bool b) {
	m_enabled = b;
}


bool GUI::Element::HandleKeyDown(int key, int mod) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(key, mod));
}

bool GUI::Element::HandleKeyUp(int key, int mod) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyUp(key, mod));
}

bool GUI::Element::HandleMouseMove(int x, int y) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleMouseMove(x + pos_x, y + pos_y));
}

bool GUI::Element::HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
	/*if (m_parent == NULL)
		return(false);
	return(m_parent->HandleMouseMove(x, y, dx ,dy));*/

	std::vector<Element*>::iterator itr = m_children.begin();

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y)) {
			if (e->isVisible())
				e->HandleMouseMove(x - e->getX(), y - e->getY(), dx ,dy);
		}
		itr++;
	}

	return(true);
}

bool GUI::Element::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);
	std::cout << name << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	std::vector<Element*>::iterator itr = m_children.begin();

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y)) {
			if (e->isVisible())
				return(e->HandleMouseDown(x - e->getX(), y - e->getY(), button));
		}
		itr++;
	}

	return(false);
}

bool GUI::Element::HandleMouseRelease(int x, int y, int button) {
	std::cout << name << "::MouseUp (" << x << ", " << y << ")" << std::endl;

	std::vector<Element*>::iterator itr = m_children.begin();

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y))
			return(e->HandleMouseRelease(x - e->getX(), y - e->getY(), button));
	}

	return(false);
}

void GUI::Element::onGetFocus()
{
	std::cout << name << "::GetFocus" << std::endl;

	Element* e = this;
	return(e->onGetFocus());
}

void GUI::Element::onLoseFocus()
{
	std::cout << name << "::LoseFocus" << std::endl;

	Element* e = this;
	return(e->onLoseFocus());
}

void GUI::Element::setActive() {
	if (m_parent != NULL) {
		m_parent->m_active_child = this;
		m_parent->setActive();
	}
}

void GUI::Element::setActiveChild(Element* e) {
	m_active_child = e;
}

GUI::Element* GUI::Element::getActiveChild() {
	return(m_active_child);
}

const GUI::Element* GUI::Element::getActiveChild() const {
	return(m_active_child);
}

GUI::Element* GUI::Element::getElement(const std::string& name) {
	return(m_elements.get(name));
}

bool GUI::Element::isFocusable() const {
	return(m_focusable);
}

bool GUI::Element::isVisible() const {
	return(m_visible);
}

bool GUI::Element::isTransparent() const {
	return(m_transparent);
}

bool GUI::Element::isEnabled() const {
	return(m_enabled);
}

void GUI::Element::SetMouseInFlag(bool flag)
{
	m_MouseIn = flag;
}
