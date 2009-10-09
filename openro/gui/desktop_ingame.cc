/* $Id: desktop_login.cc 146 2009-09-29 19:47:17Z kr105 $ */
#include "stdafx.h"

#include "desktop_ingame.h"

DesktopIngame::DesktopIngame(OpenRO* ro) : RODesktop("ui\\ingame.xml", ro) {
	ADD_HANDLER("stats_window/btnMap", evtClick, DesktopIngame::handleBtnMap);
	ADD_HANDLER("chatwindow/btnNext", evtClick, DesktopIngame::handleBtnNpcNext);
	ADD_HANDLER("chatwindow/btnCancel", evtClick, DesktopIngame::handleBtnNpcClose);

	hp = max_hp = 0;
	sp = max_sp = 0;

	minimap = (GUI::Window*)getElement("minimap");
	chatwindow = (GUI::ChatWindow*)getElement("chatwindow");
	
	m_npc_answered = false;
}

bool DesktopIngame::handleBtnNpcClose(GUI::Event&) {
	//m_ro->NpcClose();
	chatwindow->setVisible(false);
	//m_npc_answered = true;
	return(true);
}

bool DesktopIngame::handleBtnNpcNext(GUI::Event&) {
	m_ro->NpcContinue();
	m_npc_answered = true;
	return(true);
}

bool DesktopIngame::handleBtnMap(GUI::Event&) {
	minimap->setVisible(!minimap->isVisible());

	return(true);
}

void DesktopIngame::AddNpcLine(std::string line) {
	if (!chatwindow->isVisible()) {
		chatwindow->setVisible(true);
		chatwindow->Clear();
		getElement("chatwindow/btnNext")->setVisible(false);
	}
	if (m_npc_answered == true) {
		chatwindow->Clear();
		m_npc_answered = false;
	}
	chatwindow->Add(line);
}

void DesktopIngame::AddNpcNextBtn() {
	getElement("chatwindow/btnNext")->setVisible(true);
}

void DesktopIngame::afterDraw(unsigned int delay) {

	elapsed += delay;
	ptick += delay;
	ffps += delay;

	if(ffps >= 1000){
		printf("FPS: %d aprox\t\t\r", fps);
		ffps = 0;
		fps = 0;
	}else
		fps++;


	//Keep Alive packet to CharServer
	if(ptick >= 12000){
		m_ro->KeepAliveMap();
		ptick = 0;
	}
}

void DesktopIngame::setMinimap(sdle::Texture& tex) {
	minimap->setTexture(tex);
}

void DesktopIngame::SetHP(int hp, int max_hp) {
	this->hp = hp;
	this->max_hp = max_hp;
}

void DesktopIngame::SetSP(int sp, int max_sp) {
	this->sp = sp;
	this->max_sp = max_sp;
}
