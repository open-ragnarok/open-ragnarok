/* $Id$ */
#ifndef __GUI_EVENT_H
#define __GUI_EVENT_H

#include "element.h"

namespace GUI {
	class Event {
	public:
		typedef enum {
			evtClick,
			evtDoubleClick,
			evtKeyDown,
			evtKeyUp,
			evtMouseIn,
			evtMouseOut,
			evtMouseMove,
			evtUnknown
		} Type;
	protected:
		std::string m_name;
		Type m_type;
		Element* m_source;

	public:
		Event();
		Event(const Event&);
		Event(const std::string&, Type, Element* = NULL);
		virtual ~Event();

		std::string getName() const;
		std::string getSourceName() const;

		Element* getSource();
		const Element* getSource() const;

		std::string toString() const;

		Event& operator =(const Event&);
		bool operator == (const Event&) const;
		bool operator >= (const Event&) const;
		bool operator > (const Event&) const;
		bool operator <= (const Event&) const;
		bool operator < (const Event&) const;
		bool operator != (const Event&) const;

	};
};

#endif /* __GUI_EVENT_H */