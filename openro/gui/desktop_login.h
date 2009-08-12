#ifndef __DESKTOP_LOGIN_H
#define __DESKTOP_LOGIN_H

// Forward declaration
#include "ro_desktop.h"

class DesktopLogin : public RODesktop {
private:
	long penetick;
	long curtick;
	int sprc;
protected:
	GUI::TextInput* tiUser;
	GUI::TextInput* tiPass;

	virtual void afterDraw(unsigned int delay);

public:
	DesktopLogin(OpenRO*);

	bool handleBtnOk(GUI::Event&);
	bool handleBtnCancel(GUI::Event&);

	void clear();
};

#endif /* __DESKTOP_LOGIN_H */
