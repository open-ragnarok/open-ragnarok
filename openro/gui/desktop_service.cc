/* $Id$ */
#include "stdafx.h"

#include "desktop_service.h"

DesktopService::DesktopService(OpenRO* ro) : RODesktop("ui\\service_select.xml", ro) {
	ADD_HANDLER("service_select/btnOk", evtClick, DesktopService::handleBtnOk);
	ADD_HANDLER("service_select/btnCancel", evtClick, DesktopService::handleBtnCancel);

	slist = (GUI::List*)getElement("service_select/servers");
}

bool DesktopService::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.sym;

	if (key == SDLK_RETURN) {
		((GUI::Button*)getElement("service_select/btnOk"))->Click();
	}
	return(true);
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
	//m_ro->ProcessMouse();
}
