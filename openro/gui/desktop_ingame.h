#ifndef __DESKTOP_INGAME_H
#define __DESKTOP_INGAME_H

// Forward declaration
#include "ro_desktop.h"

class DesktopIngame : public RODesktop {
protected:
	virtual void afterDraw(unsigned int delay);

	unsigned short m_hp, m_maxhp;
	unsigned short m_sp, m_maxsp;
	float m_weight, m_maxweight;
	unsigned int m_zeny;

	void updateHP();
	void updateSP();
	void updateStatus();

	unsigned int elapsed;
	unsigned int ptick;
	unsigned int ffps;
	unsigned int fps;

	GUI::Window* minimap;
	GUI::ChatWindow* chatwindow;

	/** Checks if we answered the NPC. If we did, we clear the screen before writing something else in it. */
	bool m_npc_answered;

public:
	DesktopIngame(OpenRO*);

	void setMinimap(sdle::Texture&);

	bool handleBtnMap(GUI::Event&);
	bool handleBtnNpcClose(GUI::Event&);
	bool handleBtnNpcNext(GUI::Event&);
	bool handleMinimize(GUI::Event&);
	bool handleMaximize(GUI::Event&);

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

	void AddNpcLine(std::string);
	void AddNpcNextBtn();
	void AddNpcOption(std::string);
};

#endif /* __DESKTOP_LOGIN_H */
