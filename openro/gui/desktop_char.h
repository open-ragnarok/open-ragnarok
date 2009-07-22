#ifndef __DESKTOP_CHAR_H
#define __DESKTOP_CHAR_H

class DesktopChar;
#include "ro_desktop.h"
#include "ronet/structures.h"
#include "roengine/full_act.h"

class DesktopChar : public RODesktop {
private:
	void cross(float x, float y, float size);

protected:
#define CHAR_SLOT_COUNT 10
	int m_selected;
	CharInformation m_chars[CHAR_SLOT_COUNT];
	bool m_used[CHAR_SLOT_COUNT];

	GUI::Window* window;
	FullAct bodies[CHAR_SLOT_COUNT];
	FullAct heads[CHAR_SLOT_COUNT];

	virtual void afterDraw(unsigned int delay);

public:
	DesktopChar(OpenRO*);

	bool handleSelect(GUI::Event&);
	void addChar(const CharInformation&);
};

#endif /* __DESKTOP_CHAR_H */
