/* $Id: desktop_login.cc 146 2009-09-29 19:47:17Z kr105 $ */
#include "stdafx.h"

#include "desktop_login.h"

DesktopIngame::DesktopIngame(OpenRO* ro) : RODesktop("ui\\ingame.xml", ro) {
	hp = max_hp = 0;
	sp = max_sp = 0;
}

void DesktopIngame::afterDraw(unsigned int delay) {

	elapsed += delay;
	ptick += delay;

	//Keep Alive packet to CharServer
	if(ptick >= 12000){
		m_ro->KeepAliveMap();
		ptick = 0;
	}
}

void DesktopIngame::SetHP(int hp, int max_hp) {
	this->hp = hp;
	this->max_hp = max_hp;
}

void DesktopIngame::SetSP(int sp, int max_sp) {
	this->sp = sp;
	this->max_sp = max_sp;
}
