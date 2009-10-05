/* $Id$ */
#include "stdafx.h"

#include "desktop_char.h"

DesktopChar::DesktopChar(OpenRO* ro) : RODesktop("ui\\char_select.xml", ro) {
	//Add events handlers for character slots
	ADD_HANDLER("char_select/select1", evtClick, DesktopChar::handleSelect);
	ADD_HANDLER("char_select/select2", evtClick, DesktopChar::handleSelect);
	ADD_HANDLER("char_select/select3", evtClick, DesktopChar::handleSelect);

	//Add event handler for buttons
	ADD_HANDLER("char_select/btnCancel", evtClick, DesktopChar::handleCancel);
	ADD_HANDLER("char_select/btnMake", evtClick, DesktopChar::handleMake);
	ADD_HANDLER("char_select/btnOk", evtClick, DesktopChar::handleOk);

	//Add event handler for back and next buttons
	ADD_HANDLER("char_select/back", evtClick, DesktopChar::handleBack);
	ADD_HANDLER("char_select/next", evtClick, DesktopChar::handleNext);

	//Get the char_select window handler
	window = (GUI::Window*)getElement("char_select");

	//Delete all chars of the slots
	delAllChars();

	//Set the slot selected to "none"
	m_selected = -1;

	//Set the current screen to 0
	screen = 0;

	//Get label handlers
	lblStr = (GUI::Label*)getElement("char_select/str");
	lblAgi = (GUI::Label*)getElement("char_select/agi");
	lblVit = (GUI::Label*)getElement("char_select/vit");
	lblInt = (GUI::Label*)getElement("char_select/int");
	lblDex = (GUI::Label*)getElement("char_select/dex");
	lblLuk = (GUI::Label*)getElement("char_select/luk");

	//Get label handlers
	lblNam = (GUI::Label*)getElement("char_select/name");
	lblJob = (GUI::Label*)getElement("char_select/job");
	lblLvl = (GUI::Label*)getElement("char_select/level");
	lblExp = (GUI::Label*)getElement("char_select/exp");
	lblHp = (GUI::Label*)getElement("char_select/hp");
	lblSp = (GUI::Label*)getElement("char_select/sp");

	//Empty the fields of char information
	setInfo(-1);
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
	int accsex = m_ro->GetAccountSex();
	sprintf(novice_body, "sprite\\%s\\%s\\%s\\%s_%s", RO::EUC::humans, RO::EUC::body, RO::EUC::sex[accsex], RO::EUC::classname[m_chars[i].Class], RO::EUC::sex[accsex]);
	sprintf(novice_head, "sprite\\%s\\%s\\%s\\%d_%s", RO::EUC::humans, RO::EUC::head, RO::EUC::sex[accsex], m_chars[i].hair, RO::EUC::sex[accsex]);
	printf("Loading %s (%d) body in slot %d: %s\n", RO::EUC::classname_en[m_chars[i].Class], m_chars[i].Class, i, novice_body);
	printf("Loading %s (%d) head in slot %d: %s\n", RO::EUC::classname_en[m_chars[i].Class], m_chars[i].Class, i, novice_head);

	bodies[i].Load(novice_body, *m_ro);
	heads[i].Load(novice_head, *m_ro);
}

void DesktopChar::delChar(unsigned int pos) {
	m_used[pos] = false;
	return;
}

void DesktopChar::delAllChars() {
	for(int i = 0; i < CHAR_SLOT_COUNT;i++){
		delChar(i);
	}

	return;
}

void DesktopChar::setInfo(int i){
	char buf[24];
	int x = i + (screen * 3);
	
	//If the selected slot is empty
	if( m_used[x] == NULL || i == -1){
		buf[0] = 0;
		lblStr->setText(buf);
		lblAgi->setText(buf);
		lblVit->setText(buf);
		lblInt->setText(buf);
		lblDex->setText(buf);
		lblLuk->setText(buf);
		lblNam->setText(buf);
		lblJob->setText(buf);
		lblLvl->setText(buf);
		lblExp->setText(buf);
		lblHp->setText(buf);
		lblSp->setText(buf);
		return;
	}

	sprintf(buf, "%d", m_chars[x].attributes.Str);
	lblStr->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].attributes.Agi);
	lblAgi->setText(buf);

	sprintf(buf, "%d", m_chars[x].attributes.Vit);
	lblVit->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].attributes.Int);
	lblInt->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].attributes.Dex);
	lblDex->setText(buf);

	sprintf(buf, "%d", m_chars[x].attributes.Luk);
	lblLuk->setText(buf);

	sprintf(buf, "%s", m_chars[x].name);
	lblNam->setText(buf);
	
	sprintf(buf, "%s", RO::EUC::classname_en[m_chars[x].Class]);
	lblJob->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].base_lv);
	lblLvl->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].base_xp);
	lblExp->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].hp);
	lblHp->setText(buf);
	
	sprintf(buf, "%d", m_chars[x].sp);
	lblSp->setText(buf);

	memset(&buf,0x00,sizeof(buf));
}

void DesktopChar::afterDraw(unsigned int delay) {
	int i,p;
	int x[3] = { 124, 287, 450 };
	glPushMatrix();
	glTranslatef((float)window->getX(), (float)window->getY(), 0);

	p = screen * 3;

	for (i = 0; i < 3; i++) {
		if (!m_used[p+i])
			continue;

		DrawFullAct(bodies[p+i], (float)x[i], 158, 0, 0, false, NULL, true, false);
		DrawFullAct(heads[p+i], (float)x[i], 158, 0, 0, true, &bodies[p+i], true, false);
	}

	curtick = SDL_GetTicks();

	if(curtick >= (lasttick + 12000)){
		m_ro->KeepAliveChar();
		lasttick = curtick;
	}

	//m_ro->ProcessMouse(window->getX(), window->getY());

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

bool DesktopChar::setSelected(int m_selected){
	getElement("char_select/select1")->setTransparent(true);
	getElement("char_select/select2")->setTransparent(true);
	getElement("char_select/select3")->setTransparent(true);

	if (m_selected == -1)
		return(false);

	switch(m_selected){
		case 0:
			getElement("char_select/select1")->setTransparent(false);
			break;
		case 1:
			getElement("char_select/select2")->setTransparent(false);
			break;
		case 2:
			getElement("char_select/select3")->setTransparent(false);
			break;
		default:
			fprintf(stderr,"DesktopChar::setSelected() Unexpected error :S\n");
			break;
	}

	setInfo(m_selected);

	int x = m_selected + (screen * 3);
	if (m_used[x]) {
		getElement("char_select/btnMake")->setVisible(false);
		getElement("char_select/btnOk")->setVisible(true);
		getElement("char_select/btnDelete")->setVisible(true);
	}
	else {
		getElement("char_select/btnMake")->setVisible(true);
		getElement("char_select/btnOk")->setVisible(false);
		getElement("char_select/btnDelete")->setVisible(false);
	}
	return(true);
}

bool DesktopChar::handleSelect(GUI::Event& e) {
	if (e.getSourceName() == "char_select/select1") {
		m_selected = 0;
	}
	else if (e.getSourceName() == "char_select/select2") {
		m_selected = 1;
	}
	else if (e.getSourceName() == "char_select/select3") {
		m_selected = 2;
	}

	setSelected(m_selected);

	return(true);
}

bool DesktopChar::handleCancel(GUI::Event& e) {
	//Free objects
	//ROObjectCache& ro_objects = m_ro->getROObjects();
	//ro_objects.clear();

	//TODO: Delete ro_objects

	//Free char slots
	delAllChars();

	//Clean char info
	setInfo(-1);

	//Close all sockets
	m_ro->CloseSockets();

	//Change to login screen
	m_ro->LoginScreen();
	return(true);
}

/* This function handle the "Back" button in the CharSelect screen */
bool DesktopChar::handleBack(GUI::Event& e) {
	//If the selected slot is in the left
	if(m_selected == 0){
		//Set it in the right
		m_selected = 2;
		//If the current screen is the number 0
		if(screen == 0){
			//Set it in the last
			screen = ((CHAR_SLOT_COUNT / 3) - 1);
		}else{
			//Decrease the screen number
			screen -= 1;
		}
	//If nothing selected
	}else if(m_selected == -1){
		//Select the left slot
		m_selected = 0;
	}else{
		//Move the selected slot one to the left
		m_selected -= 1;
	}
	
	//Set the selected slot
	setSelected(m_selected);
	return(true);
}

/* This function handle the "Next" button in the CharSelect screen */
bool DesktopChar::handleNext(GUI::Event& e) {
	//If the selected slot is in the right
	if(m_selected == 2){
		//Set it in the left
		m_selected = 0;
		//If the current screen is the last
		if(screen == ((CHAR_SLOT_COUNT / 3) - 1)){
			//Set it in the first
			screen = 0;
		}else{
			//Increase the screen number
			screen += 1;
		}
	//If nothing selected
	}else if(m_selected == -1){
		//Select the right slot
		m_selected = 2;
	}else{
		//Move the selected slot one to the right
		m_selected += 1;
	}
	
	//Set the selected slot
	setSelected(m_selected);
	return(true);
}

bool DesktopChar::handleMake(GUI::Event& e) {
	m_ro->CreateCharWindow(m_selected + (screen * 3));
	return(true);
}

bool DesktopChar::handleOk(GUI::Event& e) {
	m_ro->CharSelect(m_selected + (screen * 3));
	return(true);
}