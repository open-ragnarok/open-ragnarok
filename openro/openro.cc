/* $Id$ */
#include "stdafx.h"
#include "openro.h"
#include "sdle/ft_font.h"
#include "rogl/actgl.h"

#ifndef _MSC_VER
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

rogl::ActGL* act_test = NULL;

OpenRO::OpenRO() : ROEngine() {
	ReadIni("data.ini");
	ReadNameTables();
	m_state = ST_Login;
	//m_showui = true;

	m_serverlist = NULL;
	m_map = NULL;
	m_maploaded = true;
	m_cycle = 0;
	m_tickoffset = 0;
}

OpenRO::~OpenRO() {
	if (m_serverlist != NULL)
		delete(m_serverlist);
}

void OpenRO::LoginScreen() {
	dskLogin->setEnabled(true);
	dskLogin->clear();
	m_state = ST_Login;
	m_gui.setDesktop(dskLogin);
	m_network.getLogin().Close();
}

void OpenRO::CharSelectScreen() {
	dskChar->setEnabled(true);
	m_state = ST_CharSelect;
	dskCreate->clear();
	m_gui.setDesktop(dskChar);
}

void OpenRO::ProcessLogin(const std::string& user, const std::string& pass, unsigned int version) {
	dskLogin->setEnabled(false);
	m_network.getLogin().Connect(m_connectionip, m_connectionport);
	bool success = m_network.GameLogin(user, pass, version);
	if (!success) {
		m_network.getLogin().Close();
		dskLogin->setEnabled(true);
	}
}

void OpenRO::ServiceSelect(unsigned int serviceid) {
	//If nothing selected
	if (serviceid < (m_serverlist->getServerCount()-1))
		return;

	dskService->setEnabled(false); //Disable the service select window
	m_network.getLogin().Close();	//Close the socket to the login server

	struct in_addr addr;
	addr.s_addr = (long)m_serverlist->getInfo(serviceid).ip;	//Convert the IP to string (stored in long)

	fprintf(stdout,"%s:%d\n",inet_ntoa(addr),m_serverlist->getInfo(serviceid).port);	//Debug info

	m_network.getChar().Connect(inet_ntoa(addr), m_serverlist->getInfo(serviceid).port);	//Connect to the charserver
	bool success = m_network.CharLogin(m_serverlist->getAccountId(), m_serverlist->getSessionId1(), m_serverlist->getSessionId2(), m_serverlist->getSex());	//Login to the charserver
	if (!success)
		dskLogin->setEnabled(true);
}

void OpenRO::CharSelect(unsigned int slot){
	//If the socket to charserver is closed
	if(!m_network.getChar().isConnected())
		return;

	dskChar->setEnabled(false);
	m_gui.setDesktop(dskLoading);

	const CharInformation& charInfo = dskChar->getChar(slot);
	printf("Char slot %d selected.\n",slot);
	m_network.CharSelect(slot);
	me.open(*this, (ro::CJob)charInfo.Class, (ro::CSex)GetAccountSex(), charInfo.hair);
	me.setSpeed(charInfo.speed);
//	me.id = charInfo.id;
	me.id = m_serverlist->getAccountId();
	dskIngame->setCharName(charInfo.name);
	dskIngame->setJob(charInfo.Class);
	dskIngame->setBaseLevel(charInfo.base_lv);
	dskIngame->setJobLevel(charInfo.job_lv);
}

void OpenRO::HandleKeyboard() {
	if (keys[SDLK_ESCAPE]) {
		keys[SDLK_ESCAPE] = false;
	//	((RODesktop*)m_gui.getDesktop())->showQuitDialog();
	/*	switch(m_state) {
		case ST_Map:
			dskIngame->showQuitDialog();
			break;
		}*/
		dskIngame->showQuitDialog();
	//	m_quit = true;
	}
	if (keys[SDLK_F10]) {
		m_gui.Dialog("Testing", "You've pressed F10", *this);
		keys[SDLK_F10] = false;
	}
}

void OpenRO::Quit() {
	switch (m_state) {
		case ST_Login:
		case ST_ServiceSelect:
		case ST_CharSelect:
		case ST_CharCreate:
			m_quit = true;
			m_state = ST_Unk;
			break;
		case ST_Loading:
		case ST_Map:
	m_network.Quit();
			break;
	}
//	m_quit = true;
//	m_state = ST_Unk;
}

void OpenRO::evtQuit() {
	Quit();
}

void OpenRO::AfterDraw() {
	if (!m_maploaded) {
		m_gui.setDesktop(dskLoading);

		if (m_cycle <= 2)
			m_cycle++;
		else {
			if (m_map != NULL) {
				delete(m_map);
				m_map = NULL;
			}

			char mapname[64];
			if (m_mapres_names.find(m_mapname) != m_mapres_names.end() )
				strcpy(mapname, m_mapres_names[m_mapname].c_str());
			else
				strcpy(mapname, m_mapname);


			m_map = RswObject::open(*this, mapname);
			if (m_map == NULL) {
				_log(OPENRO__ERROR, "Error loading map %s!", mapname);
			}
			std::string gnd_fn = m_map->getRSW()->getGndFile();
			std::string aux;
			char minimap_fn[256];

			// Loads minimap
			memset(minimap_fn, 0, 256);
			strncpy(minimap_fn, gnd_fn.c_str(), gnd_fn.length() - 3);
			strcat(minimap_fn, "bmp");

			aux = "texture\\";
			aux += ro::EUC::user_interface;
			aux += "\\map\\";
			aux += minimap_fn;
			strcpy(minimap_fn, aux.c_str());

			sdle::Texture m_minimap;
			m_minimap = m_texturemanager.Register(m_filemanager, minimap_fn);
			dskIngame->setMinimap(m_minimap);

			// Set the desktop
			m_gui.setDesktop(dskIngame);
			m_maploaded = true;
			m_cycle = 0;
			m_drawmap = true;

			// Place ourselves
			me.setMap(m_map);

			PlayBGM(m_mapname);

			// Send message to the server that we're good to go.
			m_network.MapLoaded();
		}
	}

	ProcessNetwork();
}

void OpenRO::ProcessNetwork() {
#define HANDLEPKT(pktid, delp) \
	case ronet::pkt ##pktid ##ID : \
		delpkt = delp; \
		hndl ##pktid ((ronet::pkt ##pktid *)pkt); \
		break

	m_network.Process();

	ronet::Packet* pkt = m_network.popPacket();

	bool delpkt; // if it's true, we dispose of the packet
	while (pkt != NULL) {
		delpkt = true;
		switch(pkt->getID()) {
			//Add new packets here
			HANDLEPKT(ItemGained, true);
			HANDLEPKT(ItemLost, true);
			HANDLEPKT(InventoryItems, true);
			HANDLEPKT(InventoryItemsStackable, true);
			HANDLEPKT(HpUpdateParty, true);
			HANDLEPKT(OtherSpeech, false);
			HANDLEPKT(PlayerEquip, false);
			HANDLEPKT(CharLeaveScreen, false);
			HANDLEPKT(GmBroad, true);
			HANDLEPKT(ServerTick, true);
			HANDLEPKT(AttackRange, true);
			HANDLEPKT(GuildMessage, true);
			HANDLEPKT(DisplayStat, true);
			HANDLEPKT(UpdateStatus, true);
			HANDLEPKT(ServerList, false);
			HANDLEPKT(CharList, true);
			HANDLEPKT(LoginError, true);
			HANDLEPKT(AuthFailed, true);
			HANDLEPKT(CharCreated, true);
			HANDLEPKT(CharPosition, false);
			HANDLEPKT(MapAcctSend, false);
			HANDLEPKT(MapLoginSuccess, false);
			HANDLEPKT(OwnSpeech, true);
			HANDLEPKT(SkillList, true);
			HANDLEPKT(MapMoveOk, true);
			HANDLEPKT(RecvNpcTalk, true);
			HANDLEPKT(RecvNpcTalkNext, true);
			HANDLEPKT(RecvNpcTalkClose, true);
			HANDLEPKT(RecvNpcImage2, true);
			HANDLEPKT(MapChange, true);
			HANDLEPKT(RecvNpcInputReq, true);
			HANDLEPKT(RecvNpcTalkResponses, true);
			HANDLEPKT(ZenyExp, true);
			HANDLEPKT(RestartCharSelect, true);
			HANDLEPKT(StatusUpAck, true);
			HANDLEPKT(Emotion, true);
			HANDLEPKT(StatsInfo, true);
			HANDLEPKT(QuitAck, true);
			// Actor
			HANDLEPKT(ActorDisplay, true);
			HANDLEPKT(ActorSpawn, true);
			HANDLEPKT(ActorWalking, true);
			HANDLEPKT(ActorMove, true);
			HANDLEPKT(ActorStop, true);
			HANDLEPKT(ActorAction, true);

			default:
				_log(OPENRO__ERROR, "Unhandled packet id 0x%04x (length: %d)", pkt->getID(), pkt->size());
		}

		if (delpkt)
			delete(pkt);
		pkt = m_network.popPacket();
	}
}

void OpenRO::BeforeRun() {
	ParseClientInfo();

	InitDisplay(1024, 768, false);
	InitSound();
	GLenum err = glewInit();
    if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	SDL_WM_SetCaption("Open Ragnarok - www.open-ragnarok.org", "OpenRO");

	// TODO: This should be a parameter
	sdle::FTFont* lsans = new sdle::FTFont();
	FileData data = m_filemanager.getFile("font\\lsans.ttf");
	if (data.getBuffer() == NULL || !lsans->openFromMemory(data.getBuffer(), data.blobSize(), 12)) {
		fprintf(stderr, "Erorr loading font 'font\\lsans.ttf'\n");
	}
	else {
		m_gui.FontManager().add("lsans.ttf", lsans);
		m_gui.setDefaultFont(lsans);
	}

	m_texturemanager.RegisterTGA(getFileManager(), "texture\\grid.tga");

	// Hide the mouse cursor
	SDL_ShowCursor(0);

	dskLogin = new DesktopLogin(this);
	dskService = new DesktopService(this);
	dskCreate = new DesktopCreate(this);
	dskChar = new DesktopChar(this);
	dskIngame = new DesktopIngame(this);
	dskLoading = new GUI::Desktop("ui\\loading.xml", *this);

	m_gui.setDesktop(dskLogin);

	FullAct cursor;
	char cursor_path[256];
	sprintf(cursor_path,"sprite\\cursors");
	cursor.Load(cursor_path, *this);
	setCursor(cursor);

//	char *effect = "texture\\effect\\asum.str";
//	getROObjects().ReadSTR(effect, getFileManager());
//	m_str.open(*this, (ro::STR*)getROObjects().get(effect));

	PlayBGM("bgm\\01.mp3");
}

void OpenRO::CreateCharWindow(int slot) {
	dskCreate->slot = slot;
	dskCreate->readObjects();
	m_gui.setDesktop(dskCreate);
	dskCreate->setEnabled(true);
	m_state = ST_CharCreate;
}

void OpenRO::CreateChar(const std::string& charname, const CharAttributes& attr, int slott, unsigned short color, unsigned short style) {
	printf("Creating char in slot %d\n",slott);
	dskCreate->setEnabled(false);
	m_network.CreateChar(charname, attr, slott, color, style);
	// TODO handle the exception "name is already exists"
	m_gui.setDesktop(dskChar);
}

void OpenRO::ParseClientInfo(const std::string& name){
	FileManager& fm = OpenRO::getFileManager();

	TiXmlDocument doc;
	FileData data;

	data = fm.getFile(name);
	if (data.blobSize() == 0) {
		std::cerr << "OpenRO::ParseClientInfo(): Error loading file " << name << std::endl;
	}
	doc.Parse((const char*)data.getBuffer());

	TiXmlElement* sclient_root = doc.FirstChildElement("clientinfo");
	if(sclient_root){
		TiXmlElement* sclient_child = sclient_root->FirstChildElement("connection");
		if(sclient_child){
			TiXmlElement* sclient_child_addr = sclient_child->FirstChildElement("address");
			if(sclient_child_addr){
				const char *ip = sclient_child_addr->GetText();
				strcpy(m_connectionip, ip);
			}
			TiXmlElement* sclient_child_port = sclient_child->FirstChildElement("port");
			if(sclient_child_port){
				const char *port = sclient_child_port->GetText();
				m_connectionport = atoi(port);
			}
			TiXmlElement* sclient_child_version = sclient_child->FirstChildElement("version");
			if(sclient_child_version){
				const char *version = sclient_child_version->GetText();
				m_clientversion = atoi(version);
			}
		}
	}
}

void OpenRO::CloseSockets(){
	//Close socket to Login-Server
	m_network.getLogin().Close();

	//Close socket to Char-Server
	m_network.getChar().Close();

	//Close socket to Map-Server
	m_network.getMap().Close();
	return;
}

void OpenRO::KeepAliveChar(){
	//If the socket to charserver is closed
	if(!m_network.getChar().isConnected())
		return;

	//Send the KeepAlive packet
	m_network.KeepAliveChar(m_serverlist->getAccountId());
	_log(OPENRO__DEBUG, "CharServer KeepAlive sent.");
}

void OpenRO::KeepAliveMap(){
	//If the socket to mapserver is closed
	if(!m_network.getMap().isConnected())
		return;

	//Send the KeepAlive packet
	m_network.KeepAliveMap(SDL_GetTicks() - m_tickoffset);
	//_log(OPENRO__DEBUG, "MapServer KeepAlive sent.");
}

unsigned int OpenRO::GetAccountID(){
	return m_serverlist->getAccountId();
}

unsigned char OpenRO::GetAccountSex(){
	unsigned char male = 0x01; //m
	unsigned char female = 0x00;//0x02; //f
	
	if(!m_network.getChar().isConnected())
		return 0; 
	if(m_serverlist->getSex() == male )
		return 0; 
	else if (m_serverlist->getSex() == female )
		return 1;
	else{
		_log(OPENRO__ERROR, "Error getting account sex! Defaulting to male.");
		return 0;
	}
}

unsigned int OpenRO::GetClientVersion(){
	return(m_clientversion);
}

void OpenRO::clickMap(int x, int y) {
	if (m_shift)
		me.LookAt(x, y);
	else
	m_network.MoveCharacter(x, y);
}

void OpenRO::clickNpc(int x, int y, NpcObj* npc) {
	me.LookAt(x, y);

	m_network.Talk(npc->id);
}

void OpenRO::clickMob(int x, int y, MobObj* mob) {
	me.LookAt(x, y);

//	m_network.MoveCharacter(x, y); // Need this?
	m_network.sendAction(mob->id, 7); // auto attack
//	m_network.sendAction(mob->id, 0); // attack once
	_log(OPENRO__DEBUG, "Attacking mob %d", mob->id);
}

void OpenRO::clickPortal(int x, int y, NpcObj* npc) {
//	m_network.RequestPlayerInfo(npc->id);
}

ronet::RONet& OpenRO::getNetwork() {
	return(m_network);
}

void OpenRO::NpcClose() {
	m_network.NPCCancel(m_npc_talk_id);
}

void OpenRO::NpcContinue() {
	m_network.NPCNext(m_npc_talk_id);
}

void OpenRO::NpcText(const char* text) {
	m_network.NPCText(m_npc_talk_id, text);
}

void OpenRO::NpcNumber(unsigned int n) {
	m_network.NPCNumber(m_npc_talk_id, n);
}

void OpenRO::NpcResponse(unsigned char num) {
	m_network.NPCResponse(m_npc_talk_id, num);
}

void OpenRO::LoadMap(const char* name) {
	char mapname[64];
	strcpy(mapname, name);

	for (int i = 0; mapname[i] != 0; i++) {
		if (mapname[i] == '.') {
			mapname[i] = 0;
			break;
		}
	}
	sprintf(mapname, "%s.rsw", mapname);

	// Clears all the actors
	std::map<unsigned int, Actor*>::iterator itr = m_actors.begin();
	while (itr != m_actors.end()) {
		delete(itr->second);
		itr++;
	}
	m_actors.clear();

	if (strcmp(m_mapname, mapname) == 0) {
		// We're in the same map. No need to reload stuff, but we need to
		// send to the server information that we're good to go.
		m_network.MapLoaded();
		return;
	}

	strcpy(m_mapname, mapname);

	m_maploaded = false;
	m_drawmap = false;
	StopBGM(true);
}

void OpenRO::Restart(unsigned char type) {
	me.setAction(0);
	m_network.Restart(type);
}

void OpenRO::EnableBGM(bool enabled) {
	m_bgmEnabled = enabled;
	if (enabled) {
		PlayBGM(m_mapname);
	}
	else {
		StopBGM();
	}
}

void OpenRO::EnableSE(bool enabled) {
}

void OpenRO::HandleActorInfo(const struct ActorInfo* info) {
	Actor *actor = NULL;

	if (m_actors.find(info->id) != m_actors.end()) {
		// Already on the database...
		actor = m_actors[info->id];
		actor->setVisible(true);
	}
	else if (m_npc_names.find(info->type) != m_npc_names.end()) {
		// Actor is a NPC
		NpcObj* npc = new NpcObj();
		actor = npc;
		npc->open(*this, m_npc_names[info->type]);
	//	npc->setSpeed(info->walk_speed); // Need this?
	}
	else if (m_job_names.find(info->type) != m_job_names.end()) {
		// Actor is a player
		CharObj *obj = new CharObj();
		actor = obj;
		obj->open(*this, (ro::CJob)info->type, (ro::CSex)info->sex);
//		obj->open(*this, (ro::CJob)info->type, ro::S_MALE);
		obj->setSpeed(info->walk_speed);
	}
	else if (m_homunculus_names.find(info->type) != m_homunculus_names.end()) {
		// Homunculus
		HomunObj* obj = new HomunObj();
		actor = obj;
		obj->open(*this, m_homunculus_names[info->type]);
	}
	else if (m_mercenary_names.find(info->type) != m_mercenary_names.end()) {
		// TODO: Mercenary
		NpcObj* npc = new NpcObj();
		actor = npc;
		npc->open(*this, m_npc_names[46]);
		_log(OPENRO__TRACE, "Warning: Unhandled mercenary actor type %d", info->type);
	}
	else if (info->hair_style == 0x64) {
		// TODO: Pet
		NpcObj* npc = new NpcObj();
		actor = npc;
		npc->open(*this, m_npc_names[46]);
		_log(OPENRO__TRACE, "Warning: Unhandled pet actor type %d", info->type);
	}
	else {
		// We're a monster!
		MobObj* obj = new MobObj();
		actor = obj;
		obj->open(*this, m_mob_names[info->type]);
		obj->setSpeed(info->walk_speed);
		// _log(OPENRO__TRACE, "Monster type %d (id %d)", info->type, info->id);
	}

	if (actor == NULL) {
		// Should not happen. EVER.
		_log(OPENRO__ERROR, "Invalid actor received.");
		return;
	}

	actor->typeID = info->type;
	actor->setMap(m_map);
	actor->setDirection((ro::CDir)info->dir);
	actor->id = info->id;
	actor->setPos((float)info->coord_x, (float)info->coord_y);

#if 0
	_log(OPENRO__DEBUG, "Dumping ActorInfo");
	_log(OPENRO__DEBUG, "\tID: %d", info->id);
	_log(OPENRO__DEBUG, "\tWalk speed: %d", info->walk_speed);
	_log(OPENRO__DEBUG, "\t: %d", info->opt1);
	_log(OPENRO__DEBUG, "\t: %d", info->opt2);
	_log(OPENRO__DEBUG, "\t: %d", info->option);
	_log(OPENRO__DEBUG, "\tType: %d", info->type);
	_log(OPENRO__DEBUG, "\tHair: %d", info->hair_style);
	_log(OPENRO__DEBUG, "\t: %d", info->weapon);
	_log(OPENRO__DEBUG, "\t: %d", info->lowhead);
	_log(OPENRO__DEBUG, "\t: %d", info->shield);
	_log(OPENRO__DEBUG, "\t: %d", info->tophead);
	_log(OPENRO__DEBUG, "\t: %d", info->midhead);
	_log(OPENRO__DEBUG, "\t: %d", info->hair_color);
	_log(OPENRO__DEBUG, "\t: %d", info->clothes_color);
	_log(OPENRO__DEBUG, "\tHead Direction: %d", info->head_dir);
	_log(OPENRO__DEBUG, "\tGuild: %d", info->guildID);
	_log(OPENRO__DEBUG, "\tEmblem: %d", info->emblemID);
	_log(OPENRO__DEBUG, "\t: %d", info->manner);
	_log(OPENRO__DEBUG, "\t: %d", info->opt3);
	_log(OPENRO__DEBUG, "\t: %d", info->karma);
	_log(OPENRO__DEBUG, "\tSex: %d", info->sex);
	_log(OPENRO__DEBUG, "\tCoordinates: %dx%d, %d", info->coord_x, info->coord_y, info->dir);
	_log(OPENRO__DEBUG, "\t: %d", info->unknown1);
	_log(OPENRO__DEBUG, "\t: %d", info->unknown2);
	_log(OPENRO__DEBUG, "\tAct: %d", info->act);
	_log(OPENRO__DEBUG, "\tLevel: %d", info->lv);
#endif

	m_actors[info->id] = actor;
}

/* ========================================================================== *
 * Add new packets here                                                       *
 * ========================================================================== */
HNDL_IMPL(RecvNpcTalkResponses) {
	m_npc_talk_id = pkt->getNPC();
	unsigned int s = pkt->getLineCount();
	unsigned int i;
	for (i = 0; i < s; i++) {
		dskIngame->AddNpcOption(pkt->getLine(i));
	}
}

HNDL_IMPL(ActorMove) {
	if (m_actors.find(pkt->getId()) == m_actors.end()) {
		_log(OPENRO__ERROR, "Received move request for actor id %d, but not actor found in database.", pkt->getId());
		return;
	}

	int x, y;
	Actor* actor = m_actors[pkt->getId()];
	pkt->getStart(&x, &y);
	actor->setPos((float)x, (float)y);
	pkt->getDest(&x, &y);
	actor->setDest((float)x, (float)y);
	actor->setVisible(true);
}

HNDL_IMPL(ActorStop) {
	if (me.id == pkt->getParam1()) {
		me.setPos((float)pkt->getParam2(), (float)pkt->getParam3());
		return;
	}

	if (m_actors.find(pkt->getParam1()) == m_actors.end()) {
			_log(OPENRO__ERROR, "Received stop request for actor id %d, but not actor found in database.", pkt->getParam1());
		return;
	}

	Actor* actor = m_actors[pkt->getParam1()];
	actor->setPos((float)pkt->getParam2(), (float)pkt->getParam3());
	actor->setVisible(true);
}

HNDL_IMPL(ActorAction) {
/* From OpenKore
	# type=01 pick up item
	# type=02 sit down
	# type=03 stand up
	# type=04 reflected/absorbed damage?
	# type=08 double attack
	# type=09 don't display flinch animation (endure)
	# type=0a critical hit
	# type=0b lucky dodge
*/
	switch(pkt->type) {
		case 0:
		//	printf("DAMAGE: %d->%d. Amount: %d\n", pkt->sourceID, pkt->targetID, pkt->damage);
			_log(OPENRO__DEBUG, "DAMAGE: %d->%d. Amount: %d", pkt->sourceID, pkt->targetID, pkt->damage);
			// 0: Stand 1: Walk 2: Sit 3: Pick up 4:Battle 5:Attack 6:Damage 7: 8:Dead
			if (pkt->sourceID == me.id) {
				me.Attack();
				if (pkt->damage > 0) {
					std::map<unsigned int, Actor*>::iterator itr = m_actors.find(pkt->targetID);
					if (itr != m_actors.end()) {
						itr->second->setAction(3);
					//	itr->second->Damage();
					}
				}
			}
			else {
				if (pkt->damage > 0)
					me.Damage();
				std::map<unsigned int, Actor*>::iterator itr = m_actors.find(pkt->sourceID);
				if (itr != m_actors.end()) {
					itr->second->setAction(2);
				//	itr->second->Attack();
				}
			}
			break;
		case 1:
			_log(OPENRO__DEBUG, "User %d get item %d", pkt->sourceID, pkt->targetID);
			break;
		case 2:
			_log(OPENRO__DEBUG, "User %d sits down", pkt->sourceID);
			me.setAction(2);
			break;
		case 3:
			_log(OPENRO__DEBUG, "Damage? %d->%d, damage: %d", pkt->sourceID, pkt->targetID, pkt->damage);
			me.setAction(0);
			break;
		case 0x9:
			_log(OPENRO__DEBUG, "Endure %d->%d, damage: %d", pkt->sourceID, pkt->targetID, pkt->damage);
			break;
		case 0xa:
			_log(OPENRO__DEBUG, "Critical hit %d->%d, damage: %d", pkt->sourceID, pkt->targetID, pkt->damage);
			break;
		case 0xb:
			_log(OPENRO__DEBUG, "Lucky dodge %d->%d", pkt->sourceID, pkt->targetID);
			break;
		default:
			_log(OPENRO__DEBUG, "Type: %d, source: %d, dest: %d, damage: %d", pkt->type, pkt->sourceID, pkt->targetID, pkt->damage);
	}
}

HNDL_IMPL(RecvNpcInputReq) {
	m_npc_talk_id = pkt->getID();
	dskIngame->NpcText();
}

HNDL_IMPL(RecvNpcTalk) {
	dskIngame->AddNpcLine(pkt->getMessage());
	_log(OPENRO__DEBUG, "NPC Talk: %s", pkt->getMessage());
	m_npc_talk_id = pkt->getID();
}

HNDL_IMPL(RecvNpcTalkNext) {
	dskIngame->AddNpcNextBtn();
}

HNDL_IMPL(RecvNpcTalkClose) {
	dskIngame->AddNpcCloseBtn();
}

HNDL_IMPL(RecvNpcImage2) {
	dskIngame->showNpcImage(pkt->getImageName(), pkt->getType());
	_log(OPENRO__DEBUG, "NPC Image: %s type: %d", pkt->getImageName(), pkt->getType());
}


HNDL_IMPL(ItemGained) {
	_log(OPENRO__DEBUG, "Received %d item gained",pkt->getItem().id);
}

HNDL_IMPL(ItemLost) {
	_log(OPENRO__DEBUG, "Lost item index %d amount %d",pkt->getParam1(), pkt->getParam2());
}

HNDL_IMPL(InventoryItems) {
	_log(OPENRO__DEBUG, "Received %d equipment items in inventory",pkt->getItemCount());
	for (int i = 0; i < pkt->getItemCount(); i++) {
		InventoryItem *item = pkt->getItem(i);
		_log(OPENRO__DEBUG, "Item id:%d index:%d type:%d type_equip:%d equipped:%d identified:%d", item->id, item->index, item->type, item->type_equip, item->equipped, item->identified);
		if (item->equipped > 0) {
			switch (item->equipped) {
			case 2:
				break;
			case 256:
				me.setHelm(*this, m_itemres_names[item->id].c_str());
				break;
			}
		}
	}
}

HNDL_IMPL(InventoryItemsStackable) {
	_log(OPENRO__DEBUG, "Received %d items in inventory",pkt->getItemCount());
	for (int i = 0; i < pkt->getItemCount(); i++) {
		InventoryItem *item = pkt->getItem(i);
		_log(OPENRO__DEBUG, "Item id:%d index:%d type:%d amount:%d identified:%d", item->id, item->index, item->type, item->amount, item->identified);
	}
}

HNDL_IMPL(ActorDisplay) {
	HandleActorInfo(&pkt->info);
}

HNDL_IMPL(ActorSpawn) {
	HandleActorInfo(&pkt->info);
}

HNDL_IMPL(ActorWalking) {
	HandleActorInfo(&pkt->info);
}

HNDL_IMPL(CharLeaveScreen) {
	unsigned int id = pkt->getChar();
	Actor* actor;

	std::map<unsigned int, Actor*>::iterator itr = m_actors.find(id);

	if (itr != m_actors.end()) {
		actor = itr->second;
		actor->setVisible(false);
		//delete(actor);
	}
}

HNDL_IMPL(HpUpdateParty) {
	_log(OPENRO__DEBUG, "The current HP of player %d is %d/%d",pkt->getCharId(), pkt->getHp(), pkt->getMaxHp());
}

HNDL_IMPL(OtherSpeech) {
	_log(OPENRO__DEBUG, "Player %d talk: %s",pkt->getIdMes(), pkt->getText());
	dskIngame->LogChat(pkt->getText());
}

HNDL_IMPL(PlayerEquip) {
	_log(OPENRO__DEBUG, "Player %d put %d in %d (left hand %d)", pkt->getPlayer(), pkt->getID1(), pkt->getType(), pkt->getID2());
	me.setWeapon(*this, pkt->getID1(), true);
}

HNDL_IMPL(GmBroad) {
	_log(OPENRO__DEBUG, "Received GM Broadcast: %s",pkt->getText());
}

HNDL_IMPL(ServerTick) {
	unsigned int m_expected = SDL_GetTicks() - m_tickoffset;
	unsigned int lag = pkt->getServerTick() - m_expected;
	//_log(OPENRO__DEBUG, "Received server tick: %d.\tExpected: %d.\tLag: %dms", pkt->getServerTick(), m_expected, m_lag);
	dskIngame->m_lag = lag;
	m_lag = lag;
}

HNDL_IMPL(MapMoveOk) {
	int x, y;

	pkt->getDest(&x, &y);
//	me.setDest((float)x, (float)y);
	me.WalkTo((float)x, (float)y);
}

HNDL_IMPL(AttackRange) {
	_log(OPENRO__TRACE, "Received attack range: %d", pkt->getRange());
}

HNDL_IMPL(GuildMessage) {
	_log(OPENRO__TRACE, "Guild Message: %s", pkt->getText());
	//m_network.MapLoaded();
}

HNDL_IMPL(DisplayStat) {
	unsigned int type = pkt->getType();
	unsigned int base = pkt->getBase();
	unsigned int bonus = pkt->getBonus();

	dskIngame->setStatus(type, base);
	dskIngame->setStatus(type, bonus, true);

	_log(OPENRO__TRACE, "Display Stat %s: %d + %d", ro::dnames[type], base, bonus);
}

HNDL_IMPL(UpdateStatus) {
	unsigned short type = pkt->getType();
	unsigned int value = pkt->getValue();

	switch(type) {
	//	case ro::SP_SPEED:
	//		break;
	//	case ro::SP_KARMA:
	//		break;
	//	case ro::SP_MANNER:
	//		break;
		case 5: // HP
			dskIngame->setHP(value);
			if (value == 0) {
				me.Dead();
				dskIngame->showQuitDialog();
			}
			break;
		case 6: // MAX HP
			dskIngame->setMaxHP(value);
			break;
		case 7: // SP
			dskIngame->setSP(value);
			break;
		case 8: // MAX SP
			dskIngame->setMaxSP(value);
			break;
		case ro::SP_STATUSPOINT:
			dskIngame->setStatusPoint(value);
			break;
		case ro::SP_BASELEVEL:
			dskIngame->setBaseLevel(value);
			break;
		case ro::SP_SKILLPOINT:
			dskIngame->setSkillPoint(value);
			break;
		case ro::SP_JOBLEVEL:
			dskIngame->setJobLevel(value);
			break;
		case 24: // Weight
			dskIngame->setWeight((float)value/10.0f);
			break;
		case 25: // Max Weight
			dskIngame->setMaxWeight((float)value/10.0f);
			break;
		case ro::SP_ATK1:
		case ro::SP_ATK2:
		case ro::SP_MATK1:
		case ro::SP_MATK2:
		case ro::SP_DEF1:
		case ro::SP_DEF2:
		case ro::SP_MDEF1:
		case ro::SP_MDEF2:
		case ro::SP_HIT:
		case ro::SP_FLEE1:
		case ro::SP_FLEE2:
		case ro::SP_CRITICAL:		
		case ro::SP_ASPD:
			dskIngame->setStatus(type, value);
			break;
		default:
			_log(OPENRO__TRACE, "Unhandled update status \"%s\" (%d), with value %d", ro::dnames[type], type, value);
	}
//	_log(OPENRO__TRACE, "Update status \"%s\" (%d), with value %d", ro::dnames[type], type, value);
}

HNDL_IMPL(ServerList) {
	m_serverlist = pkt;
	m_gui.setDesktop(dskService);
	dskService->setEnabled(true);
	m_state = ST_ServiceSelect;
	char buf[128];

	dskService->clear();
	for (unsigned int i = 0; i < pkt->getServerCount(); i++) {
		sprintf(buf, "%s (%d)", pkt->getInfo(i).name, pkt->getInfo(i).users);
		dskService->add(buf);
	}
}

HNDL_IMPL(CharList) {
	int count = pkt->getCount();
	_log(OPENRO__TRACE, "Received a list of %d chars", count);
	m_gui.setDesktop(dskChar);
	dskChar->setEnabled(true);
	m_state = ST_CharSelect;
	
	for(int x=0;x<count;x++){
		dskChar->addChar(pkt->getChar(x));
	}
	dskChar->setSelected(1);////
}

//[kR105]
HNDL_IMPL(LoginError) {
	short errorId = pkt->getErrorId();
	char errorDesc[256];

	//Error description taken from OpenKore & eAthena
	switch(errorId){
		case 0:
			sprintf(errorDesc,"Account name doesn't exist");
			break;
		case 1:
			sprintf(errorDesc,"Password Error");
			break;
		case 3:
			sprintf(errorDesc,"Rejected from Server");
			break;
		case 4:
			sprintf(errorDesc,"Your account has been blocked");
			break;
		case 5:
			sprintf(errorDesc,"Your Game's EXE file is not the latest version");
			break;
		case 6:
			sprintf(errorDesc,"The server is temporarily blocking your connection");
			break;
		default:
			sprintf(errorDesc,"Unknown error");
			break;
	}
	_log(OPENRO__DEBUG, "Login error: %s (Error number %d)", errorDesc, errorId);

	//We don't need the connection anymore.
	m_network.getLogin().Close();

	dskLogin->setEnabled(true);
}

//[kR105]
HNDL_IMPL(AuthFailed) {
	short errorId = pkt->getErrorId();
	char errorDesc[256];

	//Error description taken from OpenKore & eAthena
	switch(errorId){
		case 0:
			sprintf(errorDesc,"Server shutting down.");
			break;
		case 1:
			sprintf(errorDesc,"Server connection closed.");
			break;
		case 2:
			sprintf(errorDesc,"Someone has already logged in with this ID.");
			break;
		case 3:
			sprintf(errorDesc,"You've been disconnected due to a time gap between you and the server.");
			break;
		case 4:
			sprintf(errorDesc,"Server is jammed due to over population. Please try again shortly.");
			break;
		case 5:
			sprintf(errorDesc,"You are underaged and cannot join this server.");
			break;
		case 8:
			sprintf(errorDesc,"Server still recognizes your last log-in. Please try again after a few minutes.");
			break;
		case 9:
			sprintf(errorDesc,"IP capacity of this Internet Cafe is full. Would you like to pay the personal base?");
			break;
		case 10:
			sprintf(errorDesc,"You are out of available time paid for. Game will be shut down automatically.");
			break;
		case 15:
			sprintf(errorDesc,"You have been forced to disconnect by the Game Master Team.");
			break;
		default:
			sprintf(errorDesc,"Unknown error");
			break;
	}
	_log(OPENRO__DEBUG, "Auth Failed: %s (Error number %d)", errorDesc, errorId);

	//We don't need the connection anymore.
	CloseSockets();
	LoginScreen();
}

HNDL_IMPL(CharCreated) {
	unsigned short cid = pkt->getID();
	_log(OPENRO__TRACE, "Received the new character created with ID %d", cid);
	
	CharInformation newchar = pkt->getChar();
	dskChar->addChar(newchar);
}

HNDL_IMPL(CharPosition) {
	//Convert the IP to string (stored in long)
	struct in_addr addr;
	addr.s_addr = pkt->getIp();
	
	char IP[256];
	sprintf(IP,"%s",inet_ntoa(addr));

	_log(OPENRO__TRACE, "MapServer IP: %s",IP);
	_log(OPENRO__TRACE, "MapServer Port: %d",pkt->getPort());
	_log(OPENRO__TRACE, "Character Position: %s",pkt->getMapname());
	_log(OPENRO__TRACE, "Character ID: %d",pkt->getCharID());


	//Close the socket to the char server
	m_network.getChar().Close();

	//Clear buffer
	m_network.getMap().bufOutput.clear();

	//Connect to the mapserver
	m_network.getMap().Connect(IP, pkt->getPort());

	//Login to the mapserver
	m_network.MapLogin(m_serverlist->getAccountId(), pkt->getCharID(), m_serverlist->getSessionId1(), SDL_GetTicks(), m_serverlist->getSex());

	//Save the map sent by the charserver
	LoadMap(pkt->getMapname());
}

HNDL_IMPL(MapChange) {
	me.setPos((float)pkt->getPosX(), (float)pkt->getPosY());
	dskIngame->closeNpcWindow();

//	m_gui.setDesktop(dskLoading);

	LoadMap(pkt->getMapName());
}

HNDL_IMPL(MapAcctSend) {
	_log(OPENRO__TRACE, "Received accountID from mapserver: %d", pkt->getAccountId());
}

HNDL_IMPL(MapLoginSuccess) {
	short pos_x = pkt->getPosX(); 
	short pos_y = pkt->getPosY();
	short pos_dir = pkt->getPosDir();
	unsigned int server_tick = pkt->getServerTick();

	_log(OPENRO__DEBUG, "Login successful to the MapServer!");

	m_maploaded = false;
	
	me.setPos(pos_x, pos_y);
	me.setDirection((ro::CDir)pos_dir);

	m_tickoffset = SDL_GetTicks() - pkt->getServerTick();

	_log(OPENRO__TRACE, "pos_x = %d\n\t pos_y = %d\n\t pos_dir = %d\n\t server_tick = %d", pos_x, pos_y, pos_dir, server_tick);
}

HNDL_IMPL(OwnSpeech) {
	_log(OPENRO__TRACE, "OwnSpeech: %s",pkt->getText());
	dskIngame->LogChat(pkt->getText());
}

HNDL_IMPL(SkillList) {
	_log(OPENRO__TRACE, "Received skill list.");
}

HNDL_IMPL(ZenyExp) {
	switch(pkt->getType()) {
	case ro::SP_BASEEXP:
		dskIngame->addExp(pkt->getValue());
		break;
	case ro::SP_JOBEXP:
		dskIngame->addJobExp(pkt->getValue());
		break;
	case ro::SP_NEXTBASEEXP:
		dskIngame->setNextExp(pkt->getValue());
		break;
	case ro::SP_NEXTJOBEXP:
		dskIngame->setNextJobExp(pkt->getValue());
		break;
	default:
		_log(OPENRO__TRACE, "ZenyExp: Received Type:%d Value:%d", pkt->getType(), pkt->getValue());
		break;
	}
}

HNDL_IMPL(RestartCharSelect) {
	_log(OPENRO__TRACE, "Received restart char select. type: %d", pkt->getParam());

	if (m_map != NULL) {
		delete(m_map);
		m_map = NULL;
		strcpy(m_mapname, "");

		StopBGM();
	}

	//If nothing selected
//	if (serviceid < (m_serverlist->getServerCount()-1))
//		return;

	int serviceid = 0;

//	dskService->setEnabled(false); //Disable the service select window
	m_network.getMap().Close();	//Close the socket to the login server

	struct in_addr addr;
	addr.s_addr = (long)m_serverlist->getInfo(serviceid).ip;	//Convert the IP to string (stored in long)

	fprintf(stdout,"%s:%d\n",inet_ntoa(addr),m_serverlist->getInfo(serviceid).port);	//Debug info

	m_network.getChar().Connect(inet_ntoa(addr), m_serverlist->getInfo(serviceid).port);	//Connect to the charserver
	m_network.CharLogin(m_serverlist->getAccountId(), m_serverlist->getSessionId1(), m_serverlist->getSessionId2(), m_serverlist->getSex());	//Login to the charserver

//	dskIngame->setEnabled(false);
	dskChar->setEnabled(true);
	m_gui.setDesktop(dskChar);

	PlayBGM("bgm\\01.mp3");
}

HNDL_IMPL(StatusUpAck) {
	_log(OPENRO__TRACE, "Received status up. type: %d value: %d fail: %d", pkt->getParam1(), pkt->getParam3(), pkt->getParam2());
}

HNDL_IMPL(Emotion) {
	_log(OPENRO__TRACE, "Received emotion. id: %d type: %d", pkt->getParam1(), pkt->getParam2());
	if (me.id == pkt->getParam1())
		me.setEmotion(pkt->getParam2());
	else {
		std::map<unsigned int, Actor*>::iterator itr = m_actors.find(pkt->getParam1());
		if (itr != m_actors.end()) {
			itr->second->setEmotion(pkt->getParam2());
		}
	}
}

HNDL_IMPL(StatsInfo) {
	_log(OPENRO__TRACE, "Received stats info.");

	dskIngame->setStatus(ro::SP_STATUSPOINT, pkt->PointsFree);
	dskIngame->setStatus(ro::SP_STR, pkt->Str);
	dskIngame->setStatus(ro::SP_USTR, pkt->PointsStr);
	dskIngame->setStatus(ro::SP_AGI, pkt->Agi);
	dskIngame->setStatus(ro::SP_UAGI, pkt->PointsAgi);
	dskIngame->setStatus(ro::SP_VIT, pkt->Vit);
	dskIngame->setStatus(ro::SP_UVIT, pkt->PointsVit);
	dskIngame->setStatus(ro::SP_INT, pkt->Int);
	dskIngame->setStatus(ro::SP_UINT, pkt->PointsInt);
	dskIngame->setStatus(ro::SP_DEX, pkt->Dex);
	dskIngame->setStatus(ro::SP_UDEX, pkt->PointsDex);
	dskIngame->setStatus(ro::SP_LUK, pkt->Luk);
	dskIngame->setStatus(ro::SP_ULUK, pkt->PointsLuk);
	dskIngame->setStatus(ro::SP_ATK1, pkt->Attack);
	dskIngame->setStatus(ro::SP_ATK2, pkt->AttackBonus);
	dskIngame->setStatus(ro::SP_MATK1, pkt->AttackMagicMax);
	dskIngame->setStatus(ro::SP_MATK2, pkt->AttackMagicMin);
	dskIngame->setStatus(ro::SP_DEF1, pkt->Def);
	dskIngame->setStatus(ro::SP_DEF2, pkt->DefBonus);
	dskIngame->setStatus(ro::SP_MDEF1, pkt->DefMagic);
	dskIngame->setStatus(ro::SP_MDEF2, pkt->DefMagicBonus);
	dskIngame->setStatus(ro::SP_HIT, pkt->Hit);
	dskIngame->setStatus(ro::SP_FLEE1, pkt->Flee);
	dskIngame->setStatus(ro::SP_FLEE2, pkt->FleeBonus);
	dskIngame->setStatus(ro::SP_CRITICAL, pkt->Critical);
	dskIngame->setStatus(ro::SP_KARMA, pkt->Karma);
	dskIngame->setStatus(ro::SP_MANNER, pkt->Manner);
}

HNDL_IMPL(QuitAck) {
	if (pkt->getParam() == 0) {
		m_quit = true;
		m_state = ST_Unk;
	}
	else
		_log(OPENRO__TRACE, "Failed to quit.", pkt->getParam());
}

