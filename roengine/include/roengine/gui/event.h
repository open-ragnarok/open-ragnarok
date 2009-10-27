/* $Id$ */
#ifndef __GUI_EVENT_H
#define __GUI_EVENT_H

#include "element.h"

namespace GUI {

	/**
	 * GUI events that are stored on a stack for the game logic to handle.
	 */
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
			evtDialog,
			evtUnknown
		} Type;
	protected:
		/** The event name */
		std::string m_name;

		/** The event type */
		Type m_type;

		/** The event source */
		Element* m_source;

	public:
		Event();
		/** Copy constructor that make an exact copy of the parameter */
		Event(const Event&);
		Event(const std::string&, Type, Element* = NULL);
		virtual ~Event();

		std::string getName() const;
		std::string getSourceName() const;
		std::string getTypeAsString() const;

		Element* getSource();
		const Element* getSource() const;

		std::string toString() const;

		Event& operator = (const Event&);
		bool operator == (const Event&) const;
		bool operator >= (const Event&) const;
		bool operator > (const Event&) const;
		bool operator <= (const Event&) const;
		bool operator < (const Event&) const;
		bool operator != (const Event&) const;

	};
};

#endif /* __GUI_EVENT_H */
