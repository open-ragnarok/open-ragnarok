#include "stdafx.h"

#include "desktop_char.h"

DesktopChar::DesktopChar(OpenRO* ro) : RODesktop("ui\\char_select.xml", ro) {
	ADD_HANDLER("char_select/select1", evtClick, DesktopChar::handleSelect);
	ADD_HANDLER("char_select/select2", evtClick, DesktopChar::handleSelect);
	ADD_HANDLER("char_select/select3", evtClick, DesktopChar::handleSelect);

	window = (GUI::Window*)getElement("char_select");

	for (int i = 0; i < CHAR_SLOT_COUNT; i++)
		m_used[i] = false;

	m_selected = -1;
	m_used[1] = true;
	getElement("char_select/btnOk")->setVisible(false);


	TextureManager& tm = m_ro->getTextureManager();
	FileManager& fm = m_ro->getFileManager();
	ROObjectCache& ro_objects = m_ro->getROObjects();
	// body: data\sprite\<humans>\<body>\<sex>\<novice>_<sex>
	// head: data\sprite
	// 1 = archer
	// 2 = novice
	char novice_body[256];
	char novice_head[256];
	int m_headnum = 1;
	sprintf(novice_body, "sprite\\%s\\%s\\%s\\%s_%s", RO::EUC::humans, RO::EUC::body, RO::EUC::female, RO::EUC::novice, RO::EUC::female);
	sprintf(novice_head, "sprite\\%s\\%s\\%s\\%d_%s", RO::EUC::humans, RO::EUC::head, RO::EUC::female, m_headnum, RO::EUC::female);
	printf("Novice body: %s\n", novice_body);
	printf("Novice head: %s\n", novice_head);

	bodies[1].Load(novice_body, ro_objects, fm, tm);
	heads[1].Load(novice_head, ro_objects, fm, tm);
}

void DesktopChar::addChar(const CharInformation& info) {
	TextureManager& tm = m_ro->getTextureManager();
	FileManager& fm = m_ro->getFileManager();
	ROObjectCache& ro_objects = m_ro->getROObjects();

	int i = info.slot;
	m_used[i] = true;

	memcpy((char*)&m_chars[i], (char*)&info, sizeof(CharInformation));

	char novice_body[256];
	char novice_head[256];
	sprintf(novice_body, "sprite\\%s\\%s\\%s\\%s_%s", RO::EUC::humans, RO::EUC::body, RO::EUC::female, RO::EUC::classname[info.Class], RO::EUC::female);
	sprintf(novice_head, "sprite\\%s\\%s\\%s\\%d_%s", RO::EUC::humans, RO::EUC::head, RO::EUC::female, info.head_mid, RO::EUC::female);
	printf("Novice body: %s\n", novice_body);
	printf("Novice head: %s\n", novice_head);

	bodies[i].Load(novice_body, ro_objects, fm, tm);
	heads[1].Load(novice_head, ro_objects, fm, tm);
}

void DesktopChar::afterDraw(unsigned int delay) {
	int i;
	int x[3] = { 124, 287, 450 };
	glPushMatrix();
	glTranslatef((float)window->getX(), (float)window->getY(), 0);

	for (i = 0; i < 3; i++) {
		if (!m_used[i])
			continue;

		DrawFullAct(bodies[i], x[i], 158, 0, 0, false, NULL, false, true);
		DrawFullAct(heads[i], x[i], 158, 0, 0, true, &bodies[i], false, true);
	}

	glPopMatrix();
}

void DesktopChar::cross(float x, float y, float size) {
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glVertex3f(x - size, y,0);
	glVertex3f(x + size, y,0);
	glVertex3f(x, y - size,0);
	glVertex3f(x, y + size,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

bool DesktopChar::handleSelect(GUI::Event& e) {
	getElement("char_select/select1")->setTransparent(true);
	getElement("char_select/select2")->setTransparent(true);
	getElement("char_select/select3")->setTransparent(true);

	if (e.getSourceName() == "char_select/select1") {
		m_selected = 0;
		getElement("char_select/select1")->setTransparent(false);
	}
	else if (e.getSourceName() == "char_select/select2") {
		m_selected = 1;
		getElement("char_select/select2")->setTransparent(false);
	}
	else if (e.getSourceName() == "char_select/select3") {
		m_selected = 2;
		getElement("char_select/select3")->setTransparent(false);
	}

	if (m_selected == -1)
		return(false);


	if (m_used[m_selected]) {
		getElement("char_select/btnMake")->setVisible(false);
		getElement("char_select/btnOk")->setVisible(true);
	}
	else {
		getElement("char_select/btnMake")->setVisible(true);
		getElement("char_select/btnOk")->setVisible(false);
	}

	return(true);
}
