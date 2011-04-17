#ifndef __DESKTOP_INGAME_H
#define __DESKTOP_INGAME_H

// Forward declaration
#include "ro_desktop.h"

class DesktopIngame : public RODesktop {
protected:
	virtual void afterDraw(unsigned int delay);

	std::string m_charname;
	unsigned short m_job;
	unsigned short m_hp, m_maxhp;
	unsigned short m_sp, m_maxsp;
	float m_weight, m_maxweight;
	unsigned int m_zeny;
	unsigned int m_baselv, m_joblv;
	unsigned int m_baseexp, m_nextbaseexp;
	unsigned int m_jobexp, m_nextjobexp;
	unsigned int m_statuspoint;
	unsigned int m_skillpoint;
	unsigned int m_str, m_strbonus, m_strpoints;
	unsigned int m_agi, m_agibonus, m_agipoints;
	unsigned int m_vit, m_vitbonus, m_vitpoints;
	unsigned int m_int, m_intbonus, m_intpoints;
	unsigned int m_dex, m_dexbonus, m_dexpoints;
	unsigned int m_luk, m_lukbonus, m_lukpoints;
	unsigned int m_atk1, m_atk2;
	unsigned int m_matk1, m_matk2;
	unsigned int m_def1, m_def2;
	unsigned int m_mdef1, m_mdef2;
	unsigned int m_hit;
	unsigned int m_flee1, m_flee2;
	unsigned int m_cri;
	unsigned int m_aspd;
	std::string m_guildname;

	void updateMiniInfo1();
	void updateMiniInfo2();
	void updateHP();
	void updateSP();
	void updateStatus();

	unsigned int elapsed;
	unsigned int ptick;
//	unsigned int ffps;
//	unsigned int fps;
//	unsigned int m_lastfps;

	GUI::Window* minimap;
	GUI::Window* statuswindow;
	GUI::Window* optionwindow;
	GUI::Window* itemswindow;
	GUI::Window* equipwindow;
	GUI::Window* skillwindow;
//	GUI::Window* chatwindow;
	GUI::Window* friendwindow;
//	GUI::ChatWindow* chatwindow;
	GUI::TextBox* npctexts;
	GUI::Window* npcimage;
//	GUI::StaticImage* npcimage;

	GUI::Label* lblStr;
	GUI::Label* lblAgi;
	GUI::Label* lblVit;
	GUI::Label* lblInt;
	GUI::Label* lblDex;
	GUI::Label* lblLuk;
	GUI::Label* lblStr2;
	GUI::Label* lblAgi2;
	GUI::Label* lblVit2;
	GUI::Label* lblInt2;
	GUI::Label* lblDex2;
	GUI::Label* lblLuk2;
      
	GUI::Label* lblAtk;
	GUI::Label* lblMatk;
	GUI::Label* lblHit;
	GUI::Label* lblCri;
	GUI::Label* lblDef;
	GUI::Label* lblMdef;
	GUI::Label* lblFlee;
	GUI::Label* lblAspd;
	GUI::Label* lblStatusPoint;
	GUI::Label* lblGuild;

	/** Checks if we answered the NPC. If we did, we clear the screen before writing something else in it. */
	bool m_npc_answered;

public:
	unsigned int m_lag;

	DesktopIngame(OpenRO*);

	bool HandleKeyDown(SDL_Event *sdlEvent, int mod);

	void setMinimap(sdle::Texture&);

//	bool handleBtnStatus(GUI::Event&);
//	bool handleBtnMap(GUI::Event&);
	bool handleBtnStatus(GUI::Event&);
	bool handleBtnOption(GUI::Event&);
	bool handleBtnItems(GUI::Event&);
	bool handleBtnEquip(GUI::Event&);
	bool handleBtnSkill(GUI::Event&);
	bool handleBtnMap(GUI::Event&);
	bool handleBtnChat(GUI::Event&);
	bool handleBtnFriend(GUI::Event&);

	bool handleBtnNpcClose(GUI::Event&);
	bool handleBtnNpcCancel(GUI::Event&);
	bool handleBtnNpcNext(GUI::Event&);
	bool handleBtnNpcSendText(GUI::Event&);
	bool handleBtnNpcSendNumber(GUI::Event&);
	bool handleMinimize(GUI::Event&);
	bool handleMaximize(GUI::Event&);
	bool handleClose(GUI::Event&);

	bool handleBtnStatusUp(GUI::Event&);

	void handleBtnBGM(GUI::Event&);
	void handleBtnSE(GUI::Event&);
	void handleBtnOpaque(GUI::Event&);
	void handleBtnSnap(GUI::Event&);

	bool handleBtnQuit(GUI::Event&);

	void setCharName(const char* name);
	void setJob(unsigned short job);

	void setStatus(unsigned short type, unsigned int value, bool bonus = false);

	/** Sets the current HP */
	void setHP(unsigned short);

	/** Sets the current HP and the Maximum HP */
	void setHP(unsigned short, unsigned short);

	/** Sets the maximum HP */
	void setMaxHP(unsigned short);

	/** Sets the current SP */
	void setSP(unsigned short);

	/** Sets the current SP and the Maximum SP */
	void setSP(unsigned short, unsigned short);

	/** Sets the maximum SP */
	void setMaxSP(unsigned short);

	/**
	 * Sets the current Weight
	 * @param weight the current carrying weight of the user (usually the value / 10)
	 */
	void setWeight(float weight);

	/**
	 * Sets the current ana maximum Weight
	 */
	void setWeight(float, float);

	/** Sets the maximum Weight */
	void setMaxWeight(float);

	/** Sets the current Zeny */
	void setZeny(unsigned int);

	void setBaseLevel(unsigned int);
	void setJobLevel(unsigned int);
	void setStatusPoint(unsigned int);
	void setSkillPoint(unsigned int);

	void addExp(unsigned int);
	void setNextExp(unsigned int);
	void addJobExp(unsigned int);
	void setNextJobExp(unsigned int);

	void AddNpcLine(std::string);
	void AddNpcNextBtn();
	void AddNpcCloseBtn();
	void AddNpcOption(std::string);
	void showNpcImage(const char* fname, unsigned char type);
	void closeNpcWindow();

	void LogChat(std::string s);

	/** Opens up a text box for the user to input text to a NPC */
	void NpcText();

	void showQuitDialog();
};

#endif /* __DESKTOP_LOGIN_H */
