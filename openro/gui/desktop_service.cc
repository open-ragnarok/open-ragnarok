#include "stdafx.h"

#include "desktop_service.h"

DesktopService::DesktopService(OpenRO* ro) : RODesktop("ui\\service_select.xml", ro) {
	ADD_HANDLER("service_select/btnOk", evtClick, DesktopService::handleBtnOk);
	ADD_HANDLER("service_select/btnCancel", evtClick, DesktopService::handleBtnCancel);

	slist = (GUI::List*)getElement("service_select/servers");
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
	DrawFullAct(m_ro->getCursor(), (float)m_ro->getMouseX(), (float)m_ro->getMouseY(), 0, 0, false, NULL, false, true);
}