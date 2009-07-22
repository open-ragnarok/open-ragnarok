/* $Id$ */
#ifndef __GUI_DESKTOP_H
#define __GUI_DESKTOP_H

#include "window.h"
#include "event.h"

namespace GUI {
	class Desktop : public Window {
	protected:
#define ADD_HANDLER(name, evt, func) addHandler(GUI::Event(name, GUI::Event:: evt), (Handler)& func)
		typedef bool (Desktop::*Handler)(const Event&);
		bool addHandler(Event, Handler);

	private:
		std::map<std::string, Handler> m_handlers;

	public:
		Desktop();
		Desktop(const std::string&, TextureManager&, FileManager&);
		
		bool HandleEvent(const Event&);

		bool Load(const std::string&, TextureManager&, FileManager&);
		bool Load(const TiXmlElement* node, TextureManager& tm, FileManager& fm);
	};
}

#endif /* __GUI_DESKTOP_H */
