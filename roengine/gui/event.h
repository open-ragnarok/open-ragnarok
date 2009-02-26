/* $Id$ */
#ifndef __GUI_EVENT_H
#define __GUI_EVENT_H

#include "element.h"

namespace GUI {
	class Event {
	protected:
		std::string m_name;
		Element* m_source;

	public:
		Event();
		Event(const Event&);
		Event(const std::string&, Element*);
		virtual ~Event();

		std::string getName() const;
		std::string getSourceName() const;

		Element* getSource();
		const Element* getSource() const;

		Event& operator =(const Event&);
	};
};

#endif /* __GUI_EVENT_H */