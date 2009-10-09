/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/element.h"
#include "roengine/gui/gui.h"

#include <GL/gl.h>

namespace GUI {

Element::Cache Element::m_elements;

bool Element::isInside(const GUI::Element* e, int x, int y) {
	if ((x >= e->pos_x) && (y >= e->pos_y) && 
		(x <= (e->pos_x + e->w)) && (y <= (e->pos_y + e->h)))
		return(true);
	return(false);
}

bool Element::isInsideMoveArea(const GUI::Element* e, int x, int y) {
	if ((x >= (e->pos_x + e->mx)) && (y >= (e->pos_y + e->my)) &&
		(x <= (e->pos_x + e->mx + e->mw)) && (y <= (e->pos_y + e->my + e->mh)))
		return(true);
	return(false);
}

bool Element::Cache::add(GUI::Element* e) {
	if (e->getName() == "")
		e->setName(GUI::Element::createName());
	else {
		if (this->exists(e->getName())) {
			printf("Warning. Adding element with name %s Twice!\n", e->getName().c_str());
		}
	}

	return(BaseCache<Element>::add(e->getName(), e));
}

std::string Element::createName() {
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

Element::Element() {
	m_parent = NULL;
	m_focusable = true;
	m_transparent = false;
	m_stransparent = false;
	m_opacity = 1.0f;
	m_visible = true;
	m_fullscreen = false;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;
	mw = mh = 0;
	mx = my = 0;

	m_elements.add(this);
}

Element::Element(Element* parent) {
	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_stransparent = false;
	m_opacity = 1.0f;
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
	mw = mh = 0;
	mx = my = 0;

	m_elements.add(this);
}

Element::Element(Element* parent, const TiXmlElement* node, CacheManager& cache) {
	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_stransparent = false;
	m_opacity = 1.0f;
	m_visible = true;
	m_fullscreen = false;
	m_MouseIn = false;
	pos_x = 0;
	pos_y = 0;
	w = 0;
	h = 0;
	m_active_child = NULL;
	mw = mh = 0;
	mx = my = 0;

	m_elements.add(this);

	if (node != NULL)
		ParseFromXml(node, cache);
}

Element::Element(Element* parent, const std::string& background, CacheManager& cache) {
	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	m_parent = parent;
	m_focusable = true;
	m_transparent = false;
	m_stransparent = false;
	m_opacity = 1.0f;
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
	mw = mh = 0;
	mx = my = 0;

	m_elements.add(this);

	sdle::Texture ptr = tm.Register(fm, background);
	if (ptr.Valid()) {
		texture = ptr;
		setSize(texture.getWidth(), texture.getHeight());
	}
}

Element::Cache& Element::getCache() {
	return(m_elements);
}

Element::~Element() {
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

bool Element::setName(const std::string& n) {
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

const std::string& Element::getName() {
	return(name);
}

void Element::beforeDraw(unsigned int delay) {}
void Element::afterDraw(unsigned int delay) {}

void Element::Draw(unsigned int delay) {
	if (!m_transparent)
		Window(0, 0, texture);
}

void Element::Render(unsigned int delay) {
	if (!m_visible)
		return;

	glPushMatrix();
	glTranslatef((float)pos_x, (float)pos_y, 0);
	beforeDraw(delay);

	bool blend = (glIsEnabled(GL_BLEND) == 1);

	if (!blend)
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(m_stransparent){
		if(m_opacity > 0.5){
			m_opacity -= 0.006f;
			glColor4f(1.0f, 1.0f, 1.0f, m_opacity);
		}
		else {
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		}
	}
	else {
		if(m_opacity < 1) {
			m_opacity += 0.006f;
			glColor4f(1.0f, 1.0f, 1.0f, m_opacity);
		}
	}

	Draw(delay);

	std::vector<Element*>::iterator itr = m_children.begin();
	while (itr != m_children.end()) {
		(*itr)->Render(delay);
		itr++;
	}

	if (!blend)
		glDisable(GL_BLEND);

	if(m_opacity < 1) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	afterDraw(delay);
	glPopMatrix();
}

void Element::Window(float x, float y, const sdle::Texture& tp) {
	float w, h;

	w = (float)this->w;
	h = (float)this->h;

	if (m_fullscreen) {
		Gui* gui = Gui::getSingletonPtr();
		w = (float)gui->getWidth();
		h = (float)gui->getHeight();
	}
	else if (!tp.Valid()) {
		w = (float)this->w;
		h = (float)this->h;
	}
	else {
		if (w == 0)
			w = (float)tp.getWidth();
		if (h == 0)
			h = (float)tp.getHeight();
	}

	Window(x, y, w, h, tp);
}

void Element::Window(float x, float y, float w, float h, const sdle::Texture& tp) {
	float u, v;

	if (tp.Valid()) {
		tp.Activate();
		u = tp.getMaxU();
		v = tp.getMaxV();
	}
	else {
		u = 0;
		v = 0;
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, v);		glVertex3f(x,   y,   0);
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(x,   y+h, 0);
	glTexCoord2f(u, 0.0f);		glVertex3f(x+w, y+h, 0);
	glTexCoord2f(u, v);			glVertex3f(x+w, y,   0);
	glEnd();
}


void Element::add(Element* e) {
	if (e == NULL)
		return;
	m_children.push_back(e);
	if (m_active_child == NULL) {
		if (e->m_focusable) {
			m_active_child = e;
			std::vector<Element*>::iterator itr = m_active_child->m_children.begin();
			if( itr == m_active_child->m_children.end() ) {
				//std::cout << e->name << "::GetFocus" << std::endl;
				e->onGetFocus();
			}
		}
	}
}

void Element::del(const Element* e) {
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

void Element::setPos(const int& x, const int& y) {
	pos_x = x;
	pos_y = y;
}

void Element::setSize(const int& w, const int& h) {
	this->w = w;
	this->h = h;
}

bool Element::ParseXmlAttr(const TiXmlAttribute* attr, CacheManager& cache) {
	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

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
		texture = LoadTexture(attr->Value(), cache);
		return(true);
	}
	else if (attrname == "moveablewidth" || attrname == "mw") {
		if (attr->QueryIntValue(&mw) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "moveableheight" || attrname == "mh") {
		if (attr->QueryIntValue(&mh) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "moveablex" || attrname == "mx") {
		if (attr->QueryIntValue(&mx) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "moveabley" || attrname == "my") {
		if (attr->QueryIntValue(&my) != TIXML_SUCCESS) {
			printf("ERROR");
		}
		return(true);
	}
	else if (attrname == "focusable") {
		std::string val = attr->Value();
		if (val == "false" || val == "0" || val == "f")
			m_focusable = false;
		else
			m_focusable = true;
	}

	return(false);
}

sdle::Texture Element::LoadTexture(std::string name, CacheManager& cache) {
	std::string tn = name;
	if (tn[0] != '\\') {
		std::string aux = "texture\\";
		aux += RO::EUC::user_interface;
		aux += "\\";
		tn = aux + tn;
	}
	else {
		tn = tn.substr(1);
	}

	return(cache.getTextureManager().Register(cache.getFileManager(), tn));
}


void Element::ParseFromXml(const TiXmlElement* node, CacheManager& cache) {
	const TiXmlAttribute* attr = node->FirstAttribute();
	std::string attrname;

	while (attr != NULL) {
		ParseXmlAttr(attr, cache);
		attr = attr->Next();
	}

	if (w == 0 && texture.Valid())
		w = texture.getWidth();
	if (h == 0 && texture.Valid())
		h = texture.getHeight();

	if (pos_x == -1)
		CenterX();
	if (pos_y == -1)
		CenterY();
}

int Element::getX() const {
	return(pos_x);
}

int Element::getY() const {
	return(pos_y);
}

int Element::getW() const {
	if (!m_fullscreen) {
		if (w != 0)
			return(w);
		else
			if (texture.Valid())
				return(texture.getWidth());
		return(0);
	}
	return(Gui::getSingleton().getWidth());
}

int Element::getH() const {
	if (!m_fullscreen) {
		if (h != 0)
			return(h);
		else
			if (texture.Valid())
				return(texture.getHeight());
		return(0);
	}
	return(Gui::getSingleton().getHeight());
}

int Element::getMW() const {
	return(mw);
}

int Element::getMH() const {
	return(mh);
}

void Element::CenterX() {
	int pw;
	if (m_parent == NULL)
		pw = Gui::getSingleton().getWidth();
	else
		pw = m_parent->getW();

	pos_x = (pw - getW()) / 2;
}

void Element::CenterY() {
	int ph;
	if (m_parent == NULL)
		ph = Gui::getSingleton().getHeight();
	else
		ph = m_parent->getH();

	pos_y = (ph - getH()) / 2;
}

Element* Element::loadXml(Element* parent, const TiXmlElement* node, CacheManager& cache) {
	std::string nodetype = node->Value();
	GUI::Element* ret = NULL;

	if (nodetype == "window") {
		ret = new GUI::Window(parent, node, cache);
	}
	else if(nodetype == "textarea") {
		ret = new GUI::TextInput(parent, node, cache);
	}
	else if(nodetype == "label") {
		ret = new GUI::Label(parent, node, cache);
	}
	else if(nodetype == "button") {
		ret = new GUI::Button(parent, node, cache);
	}
	else if (nodetype == "list") {
		ret = new GUI::List(parent, node, cache);
	}
	else if (nodetype == "progressbar") {
		ret = new GUI::ProgressBar(parent, node, cache);
	}
	else if (nodetype == "buttonbar") {
		ret = new GUI::ButtonBar(parent, node, cache);
	}
	else if (nodetype == "chatwindow") {
		ret = new GUI::ChatWindow(parent, node, cache);
	}
	else {
		// Default
		ret = new Element(parent, node, cache);
	}

	if (ret == NULL)
		return(NULL);

	const TiXmlElement* child = node->FirstChildElement();
	GUI::Element* aux;
	while (child != NULL) {
		aux = GUI::Element::loadXml(ret, child, cache);
		if (aux != NULL)
			ret->add(aux);
		child = child->NextSiblingElement();
	}

	return(ret);
}


void Element::setTexture(const sdle::Texture& tp) {
	texture = tp;
}

void Element::setFullscreen(bool fs) {
	m_fullscreen = fs;
}

void Element::setVisible(bool b) {
	m_visible = b;
}

void Element::setTransparent(bool b) {
	m_transparent = b;
}

void Element::setStransparent(bool b) {
	m_stransparent = b;
}

void Element::setEnabled(bool b) {
	m_enabled = b;
}

bool Element::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(sdlEvent, mod));
}

bool Element::HandleKeyUp(SDL_Event *sdlEvent, int mod) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyUp(sdlEvent, mod));
}

bool Element::HandleMouseMove(int x, int y) {
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleMouseMove(x + pos_x, y + pos_y));
}

bool Element::HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
	/*if (m_parent == NULL)
		return(false);
	return(m_parent->HandleMouseMove(x, y, dx ,dy));*/

	std::vector<Element*>::iterator itr = m_children.begin();

	while (itr != m_children.end()) {
		Element* e = *itr;
		e->m_MouseIn = false;
		if (isInside(e, x, y)) {
			if (e->isVisible()) {
				e->m_MouseIn = true;
				e->HandleMouseMove(x - e->getX(), y - e->getY(), dx ,dy);
			}
		}
		itr++;
	}

	return(true);
}

bool Element::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

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

bool Element::HandleMouseRelease(int x, int y, int button) {
	//std::cout << name << "::MouseUp (" << x << ", " << y << ")" << std::endl;

	std::vector<Element*>::iterator itr = m_children.begin();

	while (itr != m_children.end()) {
		Element* e = *itr;
		if (isInside(e, x, y))
			return(e->HandleMouseRelease(x - e->getX(), y - e->getY(), button));
		itr++;
	}

	return(false);
}

void Element::onGetFocus() {
	//std::cout << name << "::GetFocus" << std::endl;

	//Element* e = this;
	//return(e->onGetFocus());
}

void Element::onLoseFocus() {
	//std::cout << name << "::LoseFocus" << std::endl;

	//Element* e = this;
	//return(e->onLoseFocus());
}

void Element::setActive() {
	if (m_parent != NULL) {
		m_parent->m_active_child = this;
		m_parent->setActive();
	}
}

void Element::setActiveChild(Element* e) {
	m_active_child = e;
}

Element* Element::getActiveChild() {
	return(m_active_child);
}

const Element* Element::getActiveChild() const {
	return(m_active_child);
}

Element* Element::getElement(const std::string& name) {
	return(m_elements.get(name));
}

bool Element::isFocusable() const {
	return(m_focusable);
}

bool Element::isVisible() const {
	return(m_visible);
}

bool Element::isTransparent() const {
	return(m_transparent);
}

bool Element::isStransparent() const {
	return(m_stransparent);
}

bool Element::isEnabled() const {
	return(m_enabled);
}

void Element::SetMouseInFlag(bool flag) {
	m_MouseIn = flag;
}

void Element::WindowSeq(float x, float y, float w, float h, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end) {
	float used_x = 0;
	float available_x;

	// Left texture
	Window(x, y, (float)start.getWidth(), (float)start.getHeight(), start);
	used_x += start.getWidth();
	
	// Checkout how much space we have
	available_x = w - used_x - end.getWidth();

	// Draw it.
	while (available_x > 0) {
		Window(x+used_x, y, (float)mid.getWidth(), (float)mid.getHeight(), mid);
		used_x += mid.getWidth();
		available_x -= mid.getWidth();
	}

	// Right edge
	Window(w - end.getWidth(), y, (float)end.getWidth(), (float)end.getHeight(), end);
}


}