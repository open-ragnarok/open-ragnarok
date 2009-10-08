#ifndef __DESKTOP_INGAME_H
#define __DESKTOP_INGAME_H

// Forward declaration
#include "ro_desktop.h"

class DesktopIngame : public RODesktop {
protected:
	virtual void afterDraw(unsigned int delay);

	int hp, max_hp;
	int sp, max_sp;

	unsigned int elapsed;
	unsigned int ptick;
	unsigned int ffps;
	unsigned int fps;

	GUI::Window* minimap;

public:
	DesktopIngame(OpenRO*);

	void setMinimap(sdle::Texture&);

	bool handleBtnMap(GUI::Event&);

	void SetHP(int hp, int max_hp);
	void SetSP(int sp, int max_sp);
};

#endif /* __DESKTOP_LOGIN_H */
