#include "stdafx.h"

#include "roengine/gui/dialog.h"
#include "roengine/gui/gui.h"

GUI::Dialog::Dialog(const std::string& title, const std::string& msg, CacheManager& cache) : Desktop() {
	ADD_HANDLER("", evtClick, Dialog::handleSelect);
	setFullscreen(false);
	setName("dialog");

	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	texture = tm.Register(fm, Gui::getSingleton().getMsgboxBg());
	if (texture.Valid()) {
		setSize(texture.getWidth(), texture.getHeight());
	}
	setMovableSize(280, 17);

	label = new Label(this, NULL, cache);
	label->setText(msg);

	btnOk = new Button(this, Gui::getSingleton().getMsgboxOk(), cache);
	btnOk->setName("dialog/btnOk");

	btnCancel = new Button(this, Gui::getSingleton().getMsgboxCancel(), cache);
	btnCancel->setName("dialog/btnCancel");

	btnExit = new Button(this, Gui::getSingleton().getMsgboxExit(), cache);
	btnExit->setName("dialog/btnExit");

	label->setPos(10, 40);

	btnOk->setPos(this->getW() - btnOk->getW() - btnCancel->getW() - 10, this->getH() - btnOk->getH() - 5);
	btnCancel->setPos(this->getW() - btnCancel->getW() - 5, this->getH() - btnCancel->getH() - 5);
	
	btnOk->setVisible(true);
	btnCancel->setVisible(true);
	btnExit->setVisible(false);

	m_children.push_back(label);
	m_children.push_back(btnOk);
	m_children.push_back(btnCancel);
	m_children.push_back(btnExit);
}

GUI::Dialog::~Dialog() {
}

bool GUI::Dialog::handleSelect(GUI::Event& e) {
	GUI::Gui& gui = GUI::Gui::getSingleton();

	if (e.getSource()->getName() == "dialog/btnOk") {
		gui.PushEvent(Event("ok", GUI::Event::evtDialog, NULL));
		setVisible(false);
		return(true);
	}
	if (e.getSource()->getName() == "dialog/btnCancel") {
		gui.PushEvent(Event("cancel", GUI::Event::evtDialog, NULL));
		setVisible(false);
		return(true);
	}
	if (e.getSource()->getName() == "dialog/btnExit") {
		gui.PushEvent(Event("exit", GUI::Event::evtDialog, NULL));
		setVisible(false);
		return(true);
	}

	return(false);
}
