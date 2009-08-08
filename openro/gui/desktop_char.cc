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

	screen = 0;

	lblStr = (GUI::Label*)getElement("char_select/str");
	lblAgi = (GUI::Label*)getElement("char_select/agi");
	lblVit = (GUI::Label*)getElement("char_select/vit");
	lblInt = (GUI::Label*)getElement("char_select/int");
	lblDex = (GUI::Label*)getElement("char_select/dex");
	lblLuk = (GUI::Label*)getElement("char_select/luk");

	lblNam = (GUI::Label*)getElement("char_select/name");
	lblJob = (GUI::Label*)getElement("char_select/job");
	lblLvl = (GUI::Label*)getElement("char_select/level");
	lblExp = (GUI::Label*)getElement("char_select/exp");
	lblHp = (GUI::Label*)getElement("char_select/hp");
	lblSp = (GUI::Label*)getElement("char_select/sp");
}
void DesktopChar::addCharScreen(unsigned int screen) {
	//In the future, this function will manage the next~back buttons of the chars list.
	//Currently, is useless.
	int c1 = screen * 3;
	int x,i;
	
	for(i = c1,x = 0;i < c1 + 3;i++,x++){
		if(m_chars[x].slot == i)
			addChar(m_chars[x]);
	}
	return;
}

void DesktopChar::addChar(const CharInformation& info) {
	TextureManager& tm = m_ro->getTextureManager();
	FileManager& fm = m_ro->getFileManager();
	ROObjectCache& ro_objects = m_ro->getROObjects();

	int i = (int)info.slot;

	m_used[i] = true;

	memcpy((char*)&m_chars[i], (char*)&info, sizeof(CharInformation));

	char novice_body[256];
	char novice_head[256];
	sprintf(novice_body, "sprite\\%s\\%s\\%s\\%s_%s", RO::EUC::humans, RO::EUC::body, RO::EUC::male, RO::EUC::classname[m_chars[i].Class], RO::EUC::male);
	sprintf(novice_head, "sprite\\%s\\%s\\%s\\%d_%s", RO::EUC::humans, RO::EUC::head, RO::EUC::male, m_chars[i].hair, RO::EUC::male);
	printf("%s (%d) body: %s\n", RO::EUC::classname_en[m_chars[i].Class], m_chars[i].Class, novice_body);
	printf("%s (%d) head: %s\n", RO::EUC::classname_en[m_chars[i].Class], m_chars[i].Class, novice_head);

	bodies[i].Load(novice_body, ro_objects, fm, tm);
	heads[i].Load(novice_head, ro_objects, fm, tm);

	//setInfo(m_chars[i].slot);

}

void DesktopChar::setInfo(unsigned int i){
	char buf[16];

	sprintf(buf, "%d", m_chars[i].attributes.Str);
	lblStr->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].attributes.Agi);
	lblAgi->setText(buf);

	sprintf(buf, "%d", m_chars[i].attributes.Vit);
	lblVit->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].attributes.Int);
	lblInt->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].attributes.Dex);
	lblDex->setText(buf);

	sprintf(buf, "%d", m_chars[i].attributes.Luk);
	lblLuk->setText(buf);

	sprintf(buf, "%s", m_chars[i].name);
	lblNam->setText(buf);
	
	sprintf(buf, "%s", RO::EUC::classname_en[m_chars[i].Class]);
	lblJob->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].base_lv);
	lblLvl->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].base_xp);
	lblExp->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].hp);
	lblHp->setText(buf);
	
	sprintf(buf, "%d", m_chars[i].sp);
	lblSp->setText(buf);
}

void DesktopChar::afterDraw(unsigned int delay) {
	int i;
	int x[3] = { 124, 287, 450 };
	glPushMatrix();
	glTranslatef((float)window->getX(), (float)window->getY(), 0);

	for (i = 0; i < 3; i++) {
		if (!m_used[i])
			continue;

		DrawFullAct(bodies[i], (float)x[i], 158, 0, 0, false, NULL, false, true);
		DrawFullAct(heads[i], (float)x[i], 158, 0, 0, true, &bodies[i], false, true);
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

	setInfo(m_selected);

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
