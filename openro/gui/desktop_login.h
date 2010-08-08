#ifndef __DESKTOP_LOGIN_H
#define __DESKTOP_LOGIN_H

// Forward declaration
#include "ro_desktop.h"

class DesktopLogin : public RODesktop {
protected:
	GUI::TextInput* tiUser;
	GUI::TextInput* tiPass;
	GUI::CheckBox* saveID;

	virtual void afterDraw(unsigned int delay);

public:
	DesktopLogin(OpenRO*);

	bool HandleKeyDown(SDL_Event *sdlEvent, int mod);

	bool handleBtnOk(GUI::Event&);
	bool handleBtnExit(GUI::Event&);
	bool handleBtnSaveID(GUI::Event&);

	void clear();
};

#endif /* __DESKTOP_LOGIN_H */
