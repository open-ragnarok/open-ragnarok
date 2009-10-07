#include "stdafx.h"

#include "roengine/gui/desktop.h"

GUI::Desktop::Desktop() : Window() {
	m_fullscreen = true;
}

GUI::Desktop::Desktop(const std::string& name, CacheManager& cache) : Window() {
	m_fullscreen = true;
	Load(name, cache);
}

bool GUI::Desktop::addHandler(Event e, Handler h) {
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

bool GUI::Desktop::HandleEvent(const Event& e) {
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

bool GUI::Desktop::Load(const std::string& name, CacheManager& cache) {
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

bool GUI::Desktop::Load(const TiXmlElement *node, CacheManager& cache) {
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
