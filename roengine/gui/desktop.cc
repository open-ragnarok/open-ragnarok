#include "stdafx.h"

#include "roengine/gui/desktop.h"

GUI::Desktop::Desktop() : Window() {
	m_fullscreen = true;
}

GUI::Desktop::Desktop(const std::string& name, TextureManager& tm, FileManager& fm) : Window() {
	m_fullscreen = true;
	Load(name, tm, fm);
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
	std::map<std::string, Handler>::iterator itr;
	std::string evtName = e.toString();
	itr = m_handlers.find(evtName);

	if (itr == m_handlers.end())
		return(false);

	Handler c = itr->second;
	return((this->*c)(e));

	return(false);
}

bool GUI::Desktop::Load(const std::string& name, TextureManager& tm, FileManager& fm) {
	TiXmlDocument doc;
	FileData data;

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

	return(Load(node, tm, fm));
}

bool GUI::Desktop::Load(const TiXmlElement *node, TextureManager &tm, FileManager &fm) {
	std::string nodetype = node->Value();
	if (nodetype != "desktop") {
		std::cerr << "Invalid node type " << nodetype << std::endl;
		return(false);
	}

	ParseFromXml(node, tm, fm);

	const TiXmlElement* child = node->FirstChildElement();
	GUI::Element* aux;
	while (child != NULL) {
		aux = GUI::Element::loadXml(this, child, tm, fm);
		if (aux != NULL)
			add(aux);
		child = child->NextSiblingElement();
	}

	return(true);
}
