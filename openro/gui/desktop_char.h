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
#define CHAR_SLOT_COUNT 9 //Only number that can be divided by 3
	int m_selected;
	CharInformation m_chars[CHAR_SLOT_COUNT];
	bool m_used[CHAR_SLOT_COUNT];

	GUI::Window* window;
	FullAct bodies[CHAR_SLOT_COUNT];
	FullAct heads[CHAR_SLOT_COUNT];

	virtual void afterDraw(unsigned int delay);

	GUI::Label* lblStr;
	GUI::Label* lblAgi;
	GUI::Label* lblVit;
	GUI::Label* lblInt;
	GUI::Label* lblDex;
	GUI::Label* lblLuk;

	GUI::Label* lblNam;
	GUI::Label* lblJob;
	GUI::Label* lblLvl;
	GUI::Label* lblExp;
	GUI::Label* lblHp;
	GUI::Label* lblSp;

public:
	DesktopChar(OpenRO*);

	bool handleSelect(GUI::Event&);
	bool handleCancel(GUI::Event&);
	void addChar(const CharInformation&);
	void addCharScreen(unsigned int screen);

	void setInfo(unsigned int i);

	unsigned int screen;
};

#endif /* __DESKTOP_CHAR_H */
