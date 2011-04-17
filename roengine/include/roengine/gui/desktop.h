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

	/** The object we're currently moving */
	Element* m_movableobj;

public:
	Desktop();
	Desktop(const std::string&, CacheManager&);
	
	virtual Type getType() { return typeDesktop; }

	bool HandleEvent(const Event&);

	bool Load(const std::string&, CacheManager&);
	bool Load(const TiXmlElement* node, CacheManager&);

	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseMove(const int& x, const int& y, const int& dx, const int& dy);
	virtual bool HandleMouseRelease(int x, int y, int button);
};

}

#endif /* __GUI_DESKTOP_H */
