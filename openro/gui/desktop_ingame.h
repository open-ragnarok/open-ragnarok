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

public:
	DesktopIngame(OpenRO*);

	void SetHP(int hp, int max_hp);
	void SetSP(int sp, int max_sp);
};

#endif /* __DESKTOP_LOGIN_H */
