#ifndef __DESKTOP_CREATE_H
#define __DESKTOP_CREATE_H

#include "ro_desktop.h"
#include "../gui_act.h"

class DesktopCreate : public RODesktop {
protected:
	unsigned int m_headnum;

	unsigned int m_pat;
	unsigned int m_action;
	unsigned int m_act;
	unsigned int m_dir;
	int elapsed;
	unsigned int ptick;

	GUI::Label* lblStr;
	GUI::Label* lblAgi;
	GUI::Label* lblVit;
	GUI::Label* lblInt;
	GUI::Label* lblDex;
	GUI::Label* lblLuk;
	GUI::Window* window;
	GUI::TextInput* txtName;

	int Str, Agi, Vit, Int, Dex, Luk;

	void updateLabels();
	virtual void afterDraw(unsigned int delay = 0);

	GuiAct obj;
	GuiAct head;

	int style;
	int color;

	void drawChar();
	void drawAct(float x, float y, GuiAct&, int pat, bool ext = false);
	void drawSpr(float x, float y, GuiAct&, int pat, int spr, bool ext = false);

public:
	DesktopCreate(OpenRO*);

	bool handleStr(GUI::Event&);
	bool handleAgi(GUI::Event&);
	bool handleVit(GUI::Event&);
	bool handleInt(GUI::Event&);
	bool handleDex(GUI::Event&);
	bool handleLuk(GUI::Event&);

	bool handleBtnColor(GUI::Event&);
	bool handleBtnStyleUp(GUI::Event&);
	bool handleBtnStyleDown(GUI::Event&);

	bool handleBtnOk(GUI::Event&);
	bool handleBtnCancel(GUI::Event&);

	int getStr() const;
	int getAgi() const;
	int getVit() const;
	int getInt() const;
	int getDex() const;
	int getLuk() const;
	const std::string& getName() const;
	void readObjects();

	void clear();

	int slot;
};

#endif /* __DESKTOP_CREATE_H */
