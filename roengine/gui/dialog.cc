#include "stdafx.h"

#include "roengine/gui/dialog.h"
#include "roengine/gui/gui.h"

GUI::Dialog::Dialog(const std::string& title, const std::string& msg, TextureManager& tm, FileManager& fm) : Desktop() {
	ADD_HANDLER("", evtClick, Dialog::handleSelect);
	setFullscreen(false);
	setName("dialog");

	texture = tm.Register(fm, Gui::getSingleton().getMsgboxBg());
	if (texture.isValid()) {
		setSize(texture->getWidth(), texture->getHeight());
	}

	btnOk = new Button(this, Gui::getSingleton().getMsgboxOk(), tm, fm);
	btnOk->setName("dialog/btnOk");

	btnCancel = new Button(this, Gui::getSingleton().getMsgboxCancel(), tm, fm);
	btnCancel->setName("dialog/btnCancel");

	btnExit = new Button(this, Gui::getSingleton().getMsgboxExit(), tm, fm);
	btnExit->setName("dialog/btnExit");

	btnOk->setPos(this->getW() - btnOk->getW() - btnCancel->getW() - 10, this->getH() - btnOk->getH() - 5);
	btnCancel->setPos(this->getW() - btnCancel->getW() - 5, this->getH() - btnCancel->getH() - 5);
	
	btnOk->setVisible(true);
	btnCancel->setVisible(true);
	btnExit->setVisible(false);

	m_children.push_back(btnOk);
	m_children.push_back(btnCancel);
	m_children.push_back(btnExit);
}

GUI::Dialog::~Dialog() {
}

bool GUI::Dialog::handleSelect(GUI::Event& e) {
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if (e.getSource()->getName() == "dialog/btnOk") {
		printf("Ok!\n");
	}

	return(false);
}
