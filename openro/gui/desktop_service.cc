#include "stdafx.h"

#include "desktop_service.h"

DesktopService::DesktopService(OpenRO* ro) : RODesktop("ui\\service_select.xml", ro) {
	ADD_HANDLER("service_select/btnOk", evtClick, DesktopService::handleBtnOk);
	ADD_HANDLER("service_select/btnCancel", evtClick, DesktopService::handleBtnCancel);

	slist = (GUI::List*)getElement("service_select/servers");

	sprc = 0;
	penetick = SDL_GetTicks();
}

bool DesktopService::handleBtnOk(GUI::Event&) {

	//If nothing selected
	if(DesktopService::getSelected() == -1)
		return(true);

	m_ro->ServiceSelect(DesktopService::getSelected());
	return(true);
}

bool DesktopService::handleBtnCancel(GUI::Event&) {
	m_ro->LoginScreen();
	return(true);
}

void DesktopService::clear() {
	slist->clear();
}

void DesktopService::add(const std::string& s) {
	slist->add(s);
}
int DesktopService::getSelected() const {
	return (slist->getSelected());
}
void DesktopService::afterDraw(unsigned int delay) {
	curtick = SDL_GetTicks();
	if(curtick >= (penetick + 100)){
		sprc++;
		penetick = curtick;
		if(sprc >= 10)
			sprc = 0;
	}
	//TODO: Put a check to know if the cursor is over a button to change the sprite to a "hand".. etc
	DrawFullAct(m_ro->getCursor(), (float)m_ro->getMouseX(), (float)m_ro->getMouseY(), 0, sprc, false, NULL, false, true);
}