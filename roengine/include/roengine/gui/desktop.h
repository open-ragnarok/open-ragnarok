/* $Id$ */
#ifndef __GUI_DESKTOP_H
#define __GUI_DESKTOP_H

#include "window.h"
#include "event.h"

namespace GUI {
class Desktop : public Window {
protected:
	/*
	 Helper to associate function handlers for the events.
	 */
#define ADD_HANDLER(name, evt, func) addHandler(GUI::Event(name, GUI::Event:: evt), (Handler)& func)
	typedef bool (Desktop::*Handler)(const Event&);
	bool addHandler(Event, Handler);

private:
	std::map<std::string, Handler> m_handlers;

public:
	Desktop();
	Desktop(const std::string&, CacheManager&);
	
	bool HandleEvent(const Event&);

	bool Load(const std::string&, CacheManager&);
	bool Load(const TiXmlElement* node, CacheManager&);

	//bool HandleMouseDown(int x, int y, int button);
};
}

#endif /* __GUI_DESKTOP_H */
