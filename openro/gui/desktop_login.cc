/* $Id$ */
#include "stdafx.h"

#include "desktop_login.h"

DesktopLogin::DesktopLogin(OpenRO* ro) : RODesktop("ui\\login.xml", ro) {
	ADD_HANDLER("login_window/btnConnect", evtClick, DesktopLogin::handleBtnOk);
	ADD_HANDLER("login_window/btnExit", evtClick, DesktopLogin::handleBtnExit);
	ADD_HANDLER("login_window/btnSaveID", evtClick, DesktopLogin::handleBtnSaveID);

	tiUser = (GUI::TextInput*)getElement("login_window/login");
	tiPass = (GUI::TextInput*)getElement("login_window/password");
	saveID = (GUI::CheckBox*)getElement("login_window/btnSaveID");
}

bool DesktopLogin::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.sym;

	if (key == SDLK_RETURN) {
		((GUI::Button*)getElement("login_window/btnConnect"))->Click();
	}
	return(true);
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

bool DesktopLogin::handleBtnExit(GUI::Event& e) {
	m_ro->Quit();
	return(true);
}

bool DesktopLogin::handleBtnSaveID(GUI::Event& e) {
	saveID->setChecked(saveID->isChecked());
	return(true);
}

void DesktopLogin::afterDraw(unsigned int delay) {
	//m_ro->ProcessMouse();
}
