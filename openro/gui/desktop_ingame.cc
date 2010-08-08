/* $Id: desktop_login.cc 146 2009-09-29 19:47:17Z kr105 $ */
#include "stdafx.h"

#include "desktop_ingame.h"

DesktopIngame::DesktopIngame(OpenRO* ro) : RODesktop("ui\\ingame.xml", ro) {
	ADD_HANDLER("stats_window/btnStatus", evtClick, DesktopIngame::handleBtnStatus);
	ADD_HANDLER("stats_window/btnOption", evtClick, DesktopIngame::handleBtnOption);
	ADD_HANDLER("stats_window/btnItems", evtClick, DesktopIngame::handleBtnItems);
	ADD_HANDLER("stats_window/btnEquip", evtClick, DesktopIngame::handleBtnEquip);
	ADD_HANDLER("stats_window/btnSkill", evtClick, DesktopIngame::handleBtnSkill);
	ADD_HANDLER("stats_window/btnMap", evtClick, DesktopIngame::handleBtnMap);
	ADD_HANDLER("stats_window/btnChat", evtClick, DesktopIngame::handleBtnChat);
	ADD_HANDLER("stats_window/btnFriend", evtClick, DesktopIngame::handleBtnFriend);

	ADD_HANDLER("npcchatwindow/btnNext", evtClick, DesktopIngame::handleBtnNpcNext);
	ADD_HANDLER("npcchatwindow/btnClose", evtClick, DesktopIngame::handleBtnNpcClose);

	ADD_HANDLER("npcchoose/btnOk", evtClick, DesktopIngame::handleBtnNpcSendNumber);
	ADD_HANDLER("npcchoose/btnCancel", evtClick, DesktopIngame::handleBtnNpcCancel);

	ADD_HANDLER("npcinput/btnOk", evtClick, DesktopIngame::handleBtnNpcSendText);
	ADD_HANDLER("npcinput/btnCancel", evtClick, DesktopIngame::handleBtnNpcClose);

	ADD_HANDLER("stats_window/btnMinimize", evtClick, DesktopIngame::handleMinimize);
	ADD_HANDLER("statuswindow/btnMinimize", evtClick, DesktopIngame::handleMinimize);
	ADD_HANDLER("optionwindow/btnMinimize", evtClick, DesktopIngame::handleMinimize);
	ADD_HANDLER("itemswindow/btnMinimize", evtClick, DesktopIngame::handleMinimize);
	ADD_HANDLER("equipwindow/btnMinimize", evtClick, DesktopIngame::handleMinimize);

	ADD_HANDLER("mini_stats/btnMinimize", evtClick, DesktopIngame::handleMaximize);

	ADD_HANDLER("statuswindow/btnClose", evtClick, DesktopIngame::handleClose);
	ADD_HANDLER("optionwindow/btnClose", evtClick, DesktopIngame::handleClose);
	ADD_HANDLER("itemswindow/btnClose", evtClick, DesktopIngame::handleClose);
	ADD_HANDLER("equipwindow/btnClose", evtClick, DesktopIngame::handleClose);
	ADD_HANDLER("skillwindow/btnClose", evtClick, DesktopIngame::handleClose);
//	ADD_HANDLER("chatwindow/btnClose", evtClick, DesktopIngame::handleClose);
//	ADD_HANDLER("friendwindow/btnClose", evtClick, DesktopIngame::handleClose);

	ADD_HANDLER("statuswindow/btnStr", evtClick, DesktopIngame::handleBtnStatusUp);
	ADD_HANDLER("statuswindow/btnAgi", evtClick, DesktopIngame::handleBtnStatusUp);
	ADD_HANDLER("statuswindow/btnVit", evtClick, DesktopIngame::handleBtnStatusUp);
	ADD_HANDLER("statuswindow/btnInt", evtClick, DesktopIngame::handleBtnStatusUp);
	ADD_HANDLER("statuswindow/btnDex", evtClick, DesktopIngame::handleBtnStatusUp);
	ADD_HANDLER("statuswindow/btnLuk", evtClick, DesktopIngame::handleBtnStatusUp);

	ADD_HANDLER("quitwindow/btnReturnSavePoint", evtClick, DesktopIngame::handleBtnQuit);
	ADD_HANDLER("quitwindow/btnCharSelect", evtClick, DesktopIngame::handleBtnQuit);
	ADD_HANDLER("quitwindow/btnExit", evtClick, DesktopIngame::handleBtnQuit);
	ADD_HANDLER("quitwindow/btnReturnGame", evtClick, DesktopIngame::handleBtnQuit);

	m_hp = m_maxhp = 0;
	m_sp = m_maxsp = 0;
	m_weight = m_maxweight = 0.0f;
	m_zeny = 0;
	m_baselv = m_joblv = 1;
	m_baseexp = m_jobexp = 0;
	m_nextbaseexp = m_nextjobexp = 1;
	m_statuspoint = 0;
	m_skillpoint = 0;
	m_str = m_strbonus = m_strpoints = 0;
	m_agi = m_agibonus = m_agipoints = 0;
	m_vit = m_vitbonus = m_vitpoints = 0;
	m_int = m_intbonus = m_intpoints = 0;
	m_dex = m_dexbonus = m_dexpoints = 0;
	m_luk = m_lukbonus = m_lukpoints = 0;
	m_atk1 = m_atk2 = 0;
	m_matk1 = m_matk2 = 0;
	m_def1 = m_def2 = 0;
	m_mdef1 = m_mdef2 = 0;
	m_hit = 0;
	m_flee1 = m_flee2 = 0;
	m_cri = 0;
	m_aspd = 0;

	minimap = (GUI::Window*)getElement("minimap");
	statuswindow = (GUI::Window*)getElement("statuswindow");
	optionwindow = (GUI::Window*)getElement("optionwindow");
	itemswindow = (GUI::Window*)getElement("itemswindow");
	equipwindow = (GUI::Window*)getElement("equipwindow");
	skillwindow = (GUI::Window*)getElement("skillwindow");
//	chatwindow = (GUI::Window*)getElement("chatwindow");
//	friendwindow = (GUI::Window*)getElement("friendwindow");

//	npcchatwindow = (GUI::npcchatwindow*)getElement("npcchatwindow");
	npctexts = (GUI::TextBox*)getElement("npcchatwindow/textbox");
	npcimage = NULL;
	
	lblStr = (GUI::Label*)getElement("statuswindow/lblStr");
	lblAgi = (GUI::Label*)getElement("statuswindow/lblAgi");
	lblVit = (GUI::Label*)getElement("statuswindow/lblVit");
	lblInt = (GUI::Label*)getElement("statuswindow/lblInt");
	lblDex = (GUI::Label*)getElement("statuswindow/lblDex");
	lblLuk = (GUI::Label*)getElement("statuswindow/lblLuk");
	lblStr2 = (GUI::Label*)getElement("statuswindow/lblStr2");
	lblAgi2 = (GUI::Label*)getElement("statuswindow/lblAgi2");
	lblVit2 = (GUI::Label*)getElement("statuswindow/lblVit2");
	lblInt2 = (GUI::Label*)getElement("statuswindow/lblInt2");
	lblDex2 = (GUI::Label*)getElement("statuswindow/lblDex2");
	lblLuk2 = (GUI::Label*)getElement("statuswindow/lblLuk2");
      
	lblAtk = (GUI::Label*)getElement("statuswindow/lblAtk");
	lblMatk = (GUI::Label*)getElement("statuswindow/lblMatk");
	lblHit = (GUI::Label*)getElement("statuswindow/lblHit");
	lblCri = (GUI::Label*)getElement("statuswindow/lblCri");
	lblDef = (GUI::Label*)getElement("statuswindow/lblDef");
	lblMdef = (GUI::Label*)getElement("statuswindow/lblMdef");
	lblFlee = (GUI::Label*)getElement("statuswindow/lblFlee");
	lblAspd = (GUI::Label*)getElement("statuswindow/lblAspd");
	lblStatusPoint = (GUI::Label*)getElement("statuswindow/lblStatusPoint");
	lblGuild = (GUI::Label*)getElement("statuswindow/lblGuild");

	m_npc_answered = false;
	m_lag = 0;
}

bool DesktopIngame::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.sym;

	if (mod & KMOD_ALT) {
		if (key >= SDLK_1 && key <= SDLK_9) {
			m_ro->getNetwork().Emotion(key - 49);
		}
		else if (key == SDLK_0) {
			m_ro->getNetwork().Emotion(9);
		}
		else if (key == SDLK_v) {
			if (getElement("stats_window")->isVisible()) {
				((GUI::Button*)getElement("stats_window/btnMinimize"))->Click();
			}
			else if (getElement("mini_stats")->isVisible()) {
				((GUI::Button*)getElement("mini_stats/btnMinimize"))->Click();
			}
		}
		else if (key == SDLK_a) {
			if (statuswindow->isVisible())
				((GUI::Button*)getElement("statuswindow/btnMinimize"))->Click();
		}
		else if (key == SDLK_e) {
			if (itemswindow->isVisible())
				((GUI::Button*)getElement("itemswindow/btnMinimize"))->Click();
		}
		else if (key == SDLK_q) {
			if (equipwindow->isVisible())
				((GUI::Button*)getElement("equipwindow/btnMinimize"))->Click();
		}
	}
	else if (mod & KMOD_CTRL) {
		if (key == SDLK_v) {
			if (getElement("stats_window")->isVisible()) {
				((GUI::Button*)getElement("stats_window/btnMinimize"))->Click();
			}
			else if (getElement("mini_stats")->isVisible()) {
				((GUI::Button*)getElement("mini_stats/btnMinimize"))->Click();
			}
		}
		else if (key == SDLK_a) {
			statuswindow->setVisible(!statuswindow->isVisible());
		}
		else if (key == SDLK_e) {
			itemswindow->setVisible(!itemswindow->isVisible());
		}
		else if (key == SDLK_q) {
			equipwindow->setVisible(!equipwindow->isVisible());
		}
	}
	else {
		if (key == SDLK_RETURN) {
			if (getElement("npcchatwindow")->isVisible()) {
				if (getElement("npcinput")->isVisible()) {
					((GUI::Button*)getElement("npcinput/btnOk"))->Click();
				}
				else if (getElement("npcchoose")->isVisible()) {
					((GUI::Button*)getElement("npcchoose/btnOk"))->Click();
				}
				else {
					if (getElement("npcchatwindow/btnNext")->isVisible())
						((GUI::Button*)getElement("npcchatwindow/btnNext"))->Click();
					else if (getElement("npcchatwindow/btnClose")->isVisible())
						((GUI::Button*)getElement("npcchatwindow/btnClose"))->Click();
				}
			}
		}
		else if (key == SDLK_SPACE) {
			if (getElement("npcchatwindow")->isVisible()) {
				if (getElement("npcchoose")->isVisible()) {
					((GUI::Button*)getElement("npcchoose/btnOk"))->Click();
				}
				else {
					if (getElement("npcchatwindow/btnNext")->isVisible())
						((GUI::Button*)getElement("npcchatwindow/btnNext"))->Click();
					else if (getElement("npcchatwindow/btnClose")->isVisible())
						((GUI::Button*)getElement("npcchatwindow/btnClose"))->Click();
				}
			}
		}
	//	else if (key == SDLK_F12) {
		else if (key == SDLK_F11) {
			getElement("shortcutwindow")->setVisible(!getElement("shortcutwindow")->isVisible());
		}
		else if (key == SDLK_INSERT) {
		//	m_network.sendAction(me.id, 2);
		//	m_ro->getNetwork().sendAction(
		}
	}

	return(true);
}

bool DesktopIngame::handleBtnNpcSendText(GUI::Event&) {
	GUI::TextInput* text = (GUI::TextInput*)getElement("npcinput/input");
	m_ro->NpcText(text->getText().c_str());
	getElement("npcinput")->setVisible(false);
	return(true);
}

bool DesktopIngame::handleBtnNpcSendNumber(GUI::Event&) {
	GUI::List* list = (GUI::List*)getElement("npcchoose/options");
	int n = list->getSelected();
	if (n < 0)
		return(false);

	m_ro->NpcResponse(n + 1);
	getElement("npcchoose")->setVisible(false);
	return(true);
}

bool DesktopIngame::handleMinimize(GUI::Event& e) {
	if (e.getSource()->getName() == "stats_window/btnMinimize") {
		int x = getElement("stats_window")->getX();
		int y = getElement("stats_window")->getY();
		getElement("mini_stats")->setPos(x, y);

		getElement("mini_stats")->setVisible(true);
		getElement("stats_window")->setVisible(false);
	}
	else {
		GUI::Element *window = NULL;
		if (e.getSource()->getName() == "statuswindow/btnMinimize") {
			window = getElement("statuswindow/statuswindow");
		}
		else if (e.getSource()->getName() == "optionwindow/btnMinimize") {
			window = getElement("optionwindow/optionwindow");
		}
		else if (e.getSource()->getName() == "itemswindow/btnMinimize") {
			window = getElement("itemswindow/itemswindow");
		}
		else if (e.getSource()->getName() == "equipwindow/btnMinimize") {
			window = getElement("equipwindow/equipwindow");
		}

		if(window != NULL)
			window->setVisible(!window->isVisible());
	}
	return(true);
}

bool DesktopIngame::handleMaximize(GUI::Event&) {
	int x = getElement("mini_stats")->getX();
	int y = getElement("mini_stats")->getY();
	getElement("stats_window")->setPos(x, y);

	getElement("mini_stats")->setVisible(false);
	getElement("stats_window")->setVisible(true);

	return(true);
}

bool DesktopIngame::handleClose(GUI::Event& e) {
	GUI::Element *window = NULL;
	if (e.getSource()->getName() == "statuswindow/btnClose") {
		window = getElement("statuswindow");
	}
	else if (e.getSource()->getName() == "optionwindow/btnClose") {
		window = getElement("optionwindow");
	}
	else if (e.getSource()->getName() == "itemswindow/btnClose") {
		window = getElement("itemswindow");
	}
	else if (e.getSource()->getName() == "equipwindow/btnClose") {
		window = getElement("equipwindow");
	}
	else if (e.getSource()->getName() == "skillwindow/btnClose") {
		window = getElement("skillwindow");
	}
	else if (e.getSource()->getName() == "npcchatwindow/btnClose") {
		window = getElement("npcchatwindow");
	}
	else if (e.getSource()->getName() == "friendwindow/btnClose") {
		window = getElement("friendwindow");
	}
	window->setVisible(false);

	return(true);
}

bool DesktopIngame::handleBtnStatusUp(GUI::Event& e) {
	short type;
	if (e.getSource()->getName() == "statuswindow/btnStr") {
		type = 0x0d;
	}
	else if (e.getSource()->getName() == "statuswindow/btnAgi") {
		type = 0x0e;
	}
	else if (e.getSource()->getName() == "statuswindow/btnVit") {
		type = 0x0f;
	}
	else if (e.getSource()->getName() == "statuswindow/btnInt") {
		type = 0x10;
	}
	else if (e.getSource()->getName() == "statuswindow/btnDex") {
		type = 0x11;
	}
	else if (e.getSource()->getName() == "statuswindow/btnLuk") {
		type = 0x12;
	}
	m_ro->getNetwork().StatusUp(type, 1);

	return(true);
}

void DesktopIngame::updateMiniInfo1() {
	char buf[100];
	sprintf(buf, "Lv. %d / %s / Lv. %d / Exp. %3.0f %%", m_baselv, ro::classname_en[m_job], m_joblv, (float)m_baseexp / m_nextbaseexp);

	GUI::Label* lbl;
	lbl = (GUI::Label*)getElement("mini_stats/lblInfo1");

	lbl->setText(buf);
}

void DesktopIngame::updateMiniInfo2() {
	char buf[100];
	sprintf(buf, "HP %d / %d | SP %d / %d | %d Z", m_hp, m_maxhp, m_sp, m_maxsp, m_zeny);

	GUI::Label* lbl;
	lbl = (GUI::Label*)getElement("mini_stats/lblInfo2");

	lbl->setText(buf);
}

void DesktopIngame::updateHP() {
	GUI::HpSpBar* bar;
	GUI::Label* lbl;
	char buf[32];

	bar = (GUI::HpSpBar*)getElement("stats_window/hp");
	lbl = (GUI::Label*)getElement("stats_window/hp_text");

	sprintf(buf, "%d / %d", m_hp, m_maxhp);

	bar->setValue(m_hp);
	bar->setMaxValue(m_maxhp);
	lbl->setText(buf);
}

void DesktopIngame::updateSP() {
	GUI::HpSpBar* bar;
	GUI::Label* lbl;
	char buf[32];

	bar = (GUI::HpSpBar*)getElement("stats_window/sp");
	lbl = (GUI::Label*)getElement("stats_window/sp_text");

	sprintf(buf, "%d / %d", m_sp, m_maxsp);

	bar->setValue(m_sp);
	bar->setMaxValue(m_maxsp);
	lbl->setText(buf);
}

void DesktopIngame::updateStatus() {
	char buf[64];
	GUI::Label* lbl = (GUI::Label*)getElement("stats_window/status");

	sprintf(buf, "Weight: %.0f / %.0f Zeny: %d", m_weight, m_maxweight, m_zeny);

	lbl->setText(buf);
}

void DesktopIngame::setCharName(const char* name) {
	m_charname = name;

	GUI::Label* lbl = (GUI::Label*)getElement("stats_window/charname");

	lbl->setText(name);

	lbl = (GUI::Label*)getElement("mini_stats/lblChar");

	lbl->setText(name);
}

void DesktopIngame::setStatus(unsigned short type, unsigned int value, bool bonus) {
	char s[20] = "";
	if (!bonus)
		_itoa(value, s, 10);

	switch(type) {
		case ro::SP_STR:
			if (!bonus)
				m_str = value;
			else
				m_strbonus = value;
			if (m_strbonus > 0)
				sprintf(s, "%2d + %d", m_str, m_strbonus);
			if (!bonus || (bonus && value > 0))
			lblStr->setText(s);
			break;
		case ro::SP_AGI:
			if (!bonus)
				m_agi = value;
			else
				m_agibonus = value;
			if (m_agibonus > 0)
				sprintf(s, "%2d + %d", m_agi, m_agibonus);
			if (!bonus || (bonus && value > 0))
			lblAgi->setText(s);
			break;
		case ro::SP_VIT:
			if (!bonus)
				m_vit = value;
			else
				m_vitbonus = value;
			if (m_vitbonus > 0)
				sprintf(s, "%2d + %d", m_vit, m_vitbonus);
			if (!bonus || (bonus && value > 0))
			lblVit->setText(s);
			break;
		case ro::SP_INT:
			if (!bonus)
				m_int = value;
			else
				m_intbonus = value;
			if (m_intbonus > 0)
				sprintf(s, "%2d + %d", m_int, m_intbonus);
			if (!bonus || (bonus && value > 0))
			lblInt->setText(s);
			break;
		case ro::SP_DEX:
			if (!bonus)
				m_dex = value;
			else
				m_dexbonus = value;
			if (m_dexbonus > 0)
				sprintf(s, "%2d + %d", m_dex, m_dexbonus);
			if (!bonus || (bonus && value > 0))
			lblDex->setText(s);
			break;
		case ro::SP_LUK:
			if (!bonus)
				m_luk = value;
			else
				m_lukbonus = value;
			if (m_lukbonus > 0)
				sprintf(s, "%2d + %d", m_luk, m_lukbonus);
			if (!bonus || (bonus && value > 0))
			lblLuk->setText(s);
			break;
		case ro::SP_USTR:
			m_strpoints = value;
			lblStr2->setText(s);
			break;
		case ro::SP_UAGI:
			m_agipoints = value;
			lblAgi2->setText(s);
			break;
		case ro::SP_UVIT:
			m_vitpoints = value;
			lblVit2->setText(s);
			break;
		case ro::SP_UINT:
			m_intpoints = value;
			lblInt2->setText(s);
			break;
		case ro::SP_UDEX:
			m_dexpoints = value;
			lblDex2->setText(s);
			break;
		case ro::SP_ULUK:		
			m_lukpoints = value;
			lblLuk2->setText(s);
			break;
		case ro::SP_STATUSPOINT:
			lblStatusPoint->setText(s);
			break;
		case ro::SP_ATK1:
			m_atk1 = value;
			sprintf(s, "%d + %d", m_atk1, m_atk2);
			lblAtk->setText(s);
			break;
		case ro::SP_ATK2:
			m_atk2 = value;
			sprintf(s, "%d + %d", m_atk1, m_atk2);
			lblAtk->setText(s);
			break;
		case ro::SP_MATK1:
			m_matk1 = value;
			sprintf(s, "%d ~ %d", m_matk2, m_matk1);
			lblMatk->setText(s);
			break;
		case ro::SP_MATK2:
			m_matk2 = value;
			sprintf(s, "%d ~ %d", m_matk2, m_matk1);
			lblMatk->setText(s);
			break;
		case ro::SP_DEF1:
			m_def1 = value;
			sprintf(s, "%d + %d", m_def1, m_def2);
			lblDef->setText(s);
			break;
		case ro::SP_DEF2:
			m_def1 = value;
			sprintf(s, "%d + %d", m_def1, m_def2);
			lblDef->setText(s);
			break;
		case ro::SP_MDEF1:
			m_mdef1 = value;
			sprintf(s, "%d + %d", m_mdef2, m_mdef1);
			lblMdef->setText(s);
			break;
		case ro::SP_MDEF2:
			m_mdef2 = value;
			sprintf(s, "%d + %d", m_mdef2, m_mdef1);
			lblMdef->setText(s);
			break;
		case ro::SP_HIT:
			m_hit = value;
			lblHit->setText(s);
			break;
		case ro::SP_FLEE1:
			m_flee1 = value;
			sprintf(s, "%d + %d", m_flee1, m_flee2);
			lblFlee->setText(s);
			break;
		case ro::SP_FLEE2:
			m_flee2 = value;
			sprintf(s, "%d + %d", m_flee1, m_flee2);
			lblFlee->setText(s);
			break;
		case ro::SP_CRITICAL:		
			lblCri->setText(s);
			break;
		case ro::SP_ASPD:
			_itoa(200 - value / 10.0, s, 10);
			lblAspd->setText(s);
			break;
		default:
			_log(OPENRO__TRACE, "Unhandled update status \"%s\" (%d), with value %d", ro::dnames[type], type, value);
	}
}

void DesktopIngame::setJob(unsigned short job) {
	m_job = job;

	GUI::Label* lbl = (GUI::Label*)getElement("stats_window/class");

	lbl->setText(ro::classname_en[job]);

	updateMiniInfo1();
}

void DesktopIngame::setHP(unsigned short hp) {
	if (hp == m_hp)
		return;
	m_hp = hp;
	updateHP();
	updateMiniInfo2();
}

void DesktopIngame::setHP(unsigned short hp, unsigned short maxhp) {
	if (hp == m_hp && maxhp == m_maxhp)
		return;
	m_hp = hp;
	m_maxhp = maxhp;
	updateHP();
	updateMiniInfo2();
}

void DesktopIngame::setMaxHP(unsigned short maxhp) {
	if (maxhp == m_maxhp)
		return;
	m_maxhp = maxhp;
	updateHP();
	updateMiniInfo2();
}

void DesktopIngame::setSP(unsigned short sp) {
	if (sp == m_sp)
		return;
	m_sp = sp;
	updateSP();
	updateMiniInfo2();
}

void DesktopIngame::setSP(unsigned short sp, unsigned short maxsp) {
	if (sp == m_sp && maxsp == m_maxsp)
		return;
	m_sp = sp;
	m_maxsp = maxsp;
	updateSP();
	updateMiniInfo2();
}

void DesktopIngame::setMaxSP(unsigned short maxsp) {
	if (maxsp == m_maxsp)
		return;
	m_maxsp = maxsp;
	updateSP();
	updateMiniInfo2();
}

void DesktopIngame::setWeight(float weight) {
	if (m_weight == weight)
		return;
	m_weight = weight;
	updateStatus();
}

void DesktopIngame::setWeight(float weight, float maxweight) {
	if (m_weight == weight && m_maxweight == maxweight)
		return;
	m_weight = weight;
	m_maxweight = maxweight;
	updateStatus();
}

void DesktopIngame::setMaxWeight(float maxweight) {
	if (m_maxweight == maxweight)
		return;
	m_maxweight = maxweight;
	updateStatus();
}

void DesktopIngame::setZeny(unsigned int zeny) {
	if (m_zeny == zeny)
		return;
	m_zeny = zeny;
	updateStatus();
	updateMiniInfo2();
}

void DesktopIngame::setBaseLevel(unsigned int baselv) {
	m_baselv = baselv;
//	updateBaseLevel();
	GUI::Label* lbl = (GUI::Label*)getElement("stats_window/baselv");
	char s[15];
	sprintf(s, "Base Lv.%d", m_baselv);
	lbl->setText(s);
}

void DesktopIngame::setJobLevel(unsigned int joblv) {
	m_joblv = joblv;
//	updateJobLevel();
	GUI::Label* lbl = (GUI::Label*)getElement("stats_window/joblv");
	char s[15];
	sprintf(s, "Job Lv.%d", m_joblv);
	lbl->setText(s);
}

void DesktopIngame::setStatusPoint(unsigned int statuspoint) {
	m_statuspoint = statuspoint;
//	updateStatus();
}

void DesktopIngame::setSkillPoint(unsigned int skillpoint) {
	m_skillpoint = skillpoint;
//	updateSkillList();
}

void DesktopIngame::addExp(unsigned int baseexp) {
	m_baseexp += baseexp;
	updateMiniInfo1();
}

void DesktopIngame::setNextExp(unsigned int nextbaseexp) {
	m_nextbaseexp = nextbaseexp;
	updateMiniInfo1();
}

void DesktopIngame::addJobExp(unsigned int jobexp) {
	m_jobexp += jobexp;
	updateMiniInfo1();
}

void DesktopIngame::setNextJobExp(unsigned int nextjobexp) {
	m_nextjobexp = nextjobexp;
	updateMiniInfo1();
}

bool DesktopIngame::handleBtnNpcClose(GUI::Event&) {
	// Hide all npc windows
//	npcchatwindow->setVisible(false);
	getElement("npcchatwindow")->setVisible(false);
	getElement("npcinput")->setVisible(false);
	getElement("npcchoose")->setVisible(false);
	m_ro->NpcClose();
	return(true);
}

bool DesktopIngame::handleBtnNpcCancel(GUI::Event&) {
	m_ro->NpcResponse(0xFF);

//	npcchatwindow->setVisible(false);
	getElement("npcchatwindow")->setVisible(false);
	getElement("npcinput")->setVisible(false);
	getElement("npcchoose")->setVisible(false);
	if(npcimage != NULL) {
		del(npcimage);
		delete npcimage;
		npcimage = NULL;
	}

	return(true);
}

bool DesktopIngame::handleBtnNpcNext(GUI::Event&) {
	m_ro->NpcContinue();
	m_npc_answered = true;
	return(true);
}
/*
bool DesktopIngame::handleBtnMap(GUI::Event&) {
	minimap->setVisible(!minimap->isVisible());

	return(true);
}

bool DesktopIngame::handleBtnStatus(GUI::Event&) {
	statusWindow->setVisible(!statusWindow->isVisible());

	return(true);
}*/

bool DesktopIngame::handleBtnStatus(GUI::Event&) {
	statuswindow->setVisible(!statuswindow->isVisible());
	if (statuswindow->isVisible())
		BringToFront(statuswindow);

	return(true);
}

bool DesktopIngame::handleBtnOption(GUI::Event&) {
	optionwindow->setVisible(!optionwindow->isVisible());
	if (optionwindow->isVisible())
		BringToFront(optionwindow);

	return(true);
}

bool DesktopIngame::handleBtnItems(GUI::Event&) {
	itemswindow->setVisible(!itemswindow->isVisible());
	if (itemswindow->isVisible())
		BringToFront(itemswindow);

	return(true);
}

bool DesktopIngame::handleBtnEquip(GUI::Event&) {
	equipwindow->setVisible(!equipwindow->isVisible());
	if (equipwindow->isVisible())
		BringToFront(equipwindow);

	return(true);
}

bool DesktopIngame::handleBtnSkill(GUI::Event&) {
	skillwindow->setVisible(!skillwindow->isVisible());
	if (skillwindow->isVisible())
		BringToFront(skillwindow);

	return(true);
}

bool DesktopIngame::handleBtnMap(GUI::Event&) {
	minimap->setVisible(!minimap->isVisible());

	return(true);
}

bool DesktopIngame::handleBtnChat(GUI::Event&) {

	return(true);
}

bool DesktopIngame::handleBtnFriend(GUI::Event&) {

	return(true);
}

void DesktopIngame::AddNpcLine(std::string line) {
	// We hide the other NPC windows when we receive new info.
	if (getElement("npcchoose")->isVisible()) {
		getElement("npcchoose")->setVisible(false);
		GUI::List* list = (GUI::List*)getElement("npcchoose/options");
		list->clear();
	}
	getElement("npcinput")->setVisible(false);

//	if (!npcchatwindow->isVisible()) {
	if (!getElement("npcchatwindow")->isVisible()) {
		getElement("npcchatwindow")->setVisible(true);
		npctexts->Clear();
		npctexts->setActive();
	/*	npcchatwindow->setVisible(true);
		npcchatwindow->Clear();
		npcchatwindow->setActive();*/
		getElement("npcchatwindow/btnClose")->setVisible(false);
		getElement("npcchatwindow/btnNext")->setVisible(false);
	}
	if (m_npc_answered == true) {
	//	npcchatwindow->Clear();
		npctexts->Clear();
		m_npc_answered = false;
	}

	getElement("npcchatwindow/btnClose")->setVisible(true);
//	npcchatwindow->Add(line);
	npctexts->Add(line);
}

void DesktopIngame::AddNpcOption(std::string line) {
	GUI::List* list = (GUI::List*)getElement("npcchoose/options");

	if (!getElement("npcchoose")->isVisible()) {
		list->clear();
	}
	getElement("npcchoose")->setVisible(true);
	// The buttons will be handled by the "npcchoose" window
	getElement("npcchatwindow/btnNext")->setVisible(false);
	getElement("npcchatwindow/btnClose")->setVisible(false);
	list->add(line);
	list->setActive();
}

void DesktopIngame::NpcText() {
	getElement("npcinput")->setVisible(true);
	getElement("npcinput/input")->setActive();
	GUI::Gui::getSingleton().setFocus(getElement("npcinput/input"));
	// The buttons will be handled by the "npcinput" window
	getElement("npcchatwindow/btnNext")->setVisible(false);
	getElement("npcchatwindow/btnClose")->setVisible(false);
}

void DesktopIngame::AddNpcNextBtn() {
	getElement("npcchatwindow/btnNext")->setVisible(true);
	getElement("npcchatwindow/btnClose")->setVisible(false);
}

void DesktopIngame::AddNpcCloseBtn() {
	getElement("npcchatwindow/btnNext")->setVisible(false);
	getElement("npcchatwindow/btnClose")->setVisible(true);
}

void DesktopIngame::showNpcImage(const char* fname, unsigned char type) {
	if (type == 2) {
		char path[256];
		sprintf(path, "texture\\%s\\illust\\%s.bmp", ro::EUC::user_interface, fname);
		npcimage = new GUI::Window(this, std::string(path), *m_ro);
		add(npcimage);
		npcimage->setPos(600, 300);
		npcimage->setVisible(true);
	}
	else if(type == 0xFF) {
		if (npcimage != NULL) {
			del(npcimage);
			delete npcimage;
			npcimage = NULL;
		}
	}
}

void DesktopIngame::closeNpcWindow() {
	getElement("npcchatwindow")->setVisible(false);
	getElement("npcinput")->setVisible(false);
	getElement("npcchoose")->setVisible(false);
}

void DesktopIngame::afterDraw(unsigned int delay) {
	elapsed += delay;
	ptick += delay;
//	ffps += delay;

	GUI::Gui& gui = GUI::Gui::getSingleton();

/*	if(ffps >= 1000){
		m_lastfps = fps;
		//printf("FPS: %d approx\t\t\r", fps);
		ffps = 0;
		fps = 0;
	}
	else {
		fps++;
	}

	char fpsbuf[36];
	sprintf(fpsbuf, "FPS: %d approx", m_lastfps);
	gui.textOut(fpsbuf, 20, 200, 0, 0);

	sprintf(fpsbuf, "Lag: %dms", m_lag);
	gui.textOut(fpsbuf, 20, 220, 0, 0);
*/

	//Keep Alive packet to CharServer
	if(ptick >= 12000){
		m_ro->KeepAliveMap();
		ptick = 0;
	}
}

void DesktopIngame::setMinimap(sdle::Texture& tex) {
	minimap->setTexture(tex);
}

void DesktopIngame::showQuitDialog() {
//	getElement("quitwindow")->setVisible(true);
	getElement("quitwindow")->setVisible(!getElement("quitwindow")->isVisible());
	BringToFront(getElement("quitwindow"));
}

bool DesktopIngame::handleBtnQuit(GUI::Event& e) {
	if (e.getSource()->getName() == "quitwindow/btnReturnSavePoint") {
		getElement("quitwindow")->setVisible(false);
		m_ro->Restart(0);
		return(true);
	}
	else if (e.getSource()->getName() == "quitwindow/btnCharSelect") {
	//	setVisible(false);
		m_ro->Restart(1);
		return(true);
	}
	else if (e.getSource()->getName() == "quitwindow/btnExit") {
		m_ro->Quit();
		return(true);
	}
	else if (e.getSource()->getName() == "quitwindow/btnReturnGame") {
		getElement("quitwindow")->setVisible(false);
		return(true);
	}

	return(true);
}