/* $Id$ */
#include "stdafx.h"

#include "desktop_login.h"

DesktopLogin::DesktopLogin(OpenRO* ro) : RODesktop("ui\\login.xml", ro) {
	ADD_HANDLER("login_window/btnConnect", evtClick, DesktopLogin::handleBtnOk);
	ADD_HANDLER("login_window/btnCancel", evtClick, DesktopLogin::handleBtnCancel);

	tiUser = (GUI::TextInput*)getElement("login_window/login");
	tiPass = (GUI::TextInput*)getElement("login_window/password");
}

bool DesktopLogin::handleBtnOk(GUI::Event& e) {
	std::string user, pass;

	user = tiUser->getText();
	pass = tiPass->getText();

	m_ro->ProcessLogin(user, pass, m_ro->GetClientVersion());
	return(true);
}

void DesktopLogin::clear() {
	tiUser->setText("");
	tiPass->setText("");
}

bool DesktopLogin::handleBtnCancel(GUI::Event& e) {
	m_ro->Quit();
	return(true);
}

void DesktopLogin::afterDraw(unsigned int delay) {
	//m_ro->ProcessMouse();
}
