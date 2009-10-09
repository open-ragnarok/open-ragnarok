#include "stdafx.h"

#include "roengine/gui/desktop.h"
#include "roengine/gui/gui.h"

namespace GUI {

Desktop::Desktop() : Window() {
	m_fullscreen = true;
	m_movableobj = NULL;
}

Desktop::Desktop(const std::string& name, CacheManager& cache) : Window() {
	m_fullscreen = true;
	m_movableobj = NULL;
	Load(name, cache);
}

bool Desktop::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	std::vector<Element*>::iterator itr = m_children.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	// Handle window movement
	if (this == gui.getDesktop()) {
		while (itr != m_children.end()) {
			Element* e = *itr;

			if (button == 1 && isInside(e, x, y) && e->isVisible() && isInsideMoveArea(e, x, y) ) {
				//std::cout << getName() << "::MouseDownOnWindowMoveArea (" << x << ", " << y << ")" << std::endl;
				m_movableobj = e;
				m_movableobj->setStransparent(true);
				// We're moving the window. There's nothing else to do.
				return(true);
			}
			itr++;
		}
	}

	// Let the element handle that for us...
	return(Element::HandleMouseDown(x, y, button));
}

bool Desktop::HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
	if (!m_enabled)
		return(false);

	if (m_movableobj != NULL) {
		m_movableobj->setPos(m_movableobj->getX() + dx, m_movableobj->getY() + dy);
		return(true);
	}

	return(Element::HandleMouseMove(x, y, dx, dy));
}

bool Desktop::HandleMouseRelease(int x, int y, int button) {
	std::vector<Element*>::iterator itr = m_children.begin();
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if (m_movableobj != NULL) {
		m_movableobj->setStransparent(false);
		m_movableobj = NULL;
		return(true);
	}

	return(Element::HandleMouseRelease(x, y, button));
}


bool Desktop::addHandler(Event e, Handler h) {
	std::string evtName = e.toString();
	std::map<std::string, Handler>::iterator itr = m_handlers.find(evtName);
	if (itr != m_handlers.end()) {
		printf("Replacing handler!");
		itr->second = h;
		return(true);
	}
	m_handlers[evtName] = h;
	return(true);
}

bool Desktop::HandleEvent(const Event& e) {
	// Try to find an registered handler with the event name.
	std::map<std::string, Handler>::iterator itr;
	std::string evtName = e.toString();
	itr = m_handlers.find(evtName);

	if (itr == m_handlers.end()) {
		// No event registered for the object source. Check if we have a generic handler
		// for the event (a handler with an empty name).
		evtName = "::";
		evtName += e.getTypeAsString();
		itr = m_handlers.find(evtName);
		if (itr == m_handlers.end()) {
			return(false);
		}
	}

	Handler c = itr->second;
	return((this->*c)(e));
}

bool Desktop::Load(const std::string& name, CacheManager& cache) {
	TiXmlDocument doc;
	FileData data;

	FileManager& fm = cache.getFileManager();

	data = fm.getFile(name);
	if (data.blobSize() == 0) {
		std::cerr << "GUI::Desktop::Load(): Error loading file " << name << std::endl;
		return(false);
	}
	doc.Parse((const char*)data.getBuffer());

	TiXmlElement* node;
	node = doc.FirstChildElement();
	if (node == NULL)
		return(false);

	return(Load(node, cache));
}

bool Desktop::Load(const TiXmlElement *node, CacheManager& cache) {
	std::string nodetype = node->Value();
	if (nodetype != "desktop") {
		std::cerr << "Invalid node type " << nodetype << std::endl;
		return(false);
	}

	ParseFromXml(node, cache);

	const TiXmlElement* child = node->FirstChildElement();
	GUI::Element* aux;
	while (child != NULL) {
		aux = GUI::Element::loadXml(this, child, cache);
		if (aux != NULL)
			add(aux);
		child = child->NextSiblingElement();
	}

	return(true);
}

}
