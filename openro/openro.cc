/* $Id$ */
#include "stdafx.h"
#include "openro.h"
#include "sdle/ft_font.h"
#include "rogl/actgl.h"

rogl::ActGL* act_test = NULL;

OpenRO::OpenRO() : ROEngine() {
	ReadIni("data.ini");
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
	dskCreate->clear();
	m_gui.setDesktop(dskChar);
}

void OpenRO::ProcessLogin(const std::string& user, const std::string& pass, unsigned int version) {
	dskLogin->setEnabled(false);
	m_network.getLogin().Connect(OpenRO::ConnectionIP, OpenRO::ConnectionPort);
	m_network.GameLogin(user, pass, version);
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
	m_network.CharLogin(m_serverlist->getAccountId(), m_serverlist->getSessionId1(), m_serverlist->getSessionId2(), m_serverlist->getSex());	//Login to the charserver
}

void OpenRO::CharSelect(unsigned int slot){
	//If the socket to charserver is closed
	if(!m_network.getChar().isConnected())
		return;

	dskChar->setEnabled(false);

	printf("Char slot %d selected.\n",slot);
	m_network.CharSelect(slot);
}

void OpenRO::HandleKeyboard() {
	if (keys[SDLK_ESCAPE])
		m_quit = true;
	if (keys[SDLK_F10]) {
		m_gui.Dialog("Testing", "You've pressed F10", *this);
		keys[SDLK_F10] = false;
	}
}

void OpenRO::Quit() {
	m_quit = true;
	m_state = ST_Unk;
}

void OpenRO::AfterDraw() {
	if (!m_maploaded) {
		if (!m_cycle)
			m_cycle++;
		else {
			m_map = RswObject::open(*this, m_mapname);
			std::string gnd_fn = m_map->getRSW()->gnd_file;
			std::string aux;
			char minimap_fn[256];

			memset(minimap_fn, 0, 256);
			strncpy(minimap_fn, gnd_fn.c_str(), gnd_fn.length() - 3);
			strcat(minimap_fn, "bmp");

			aux = "texture\\";
			aux += RO::EUC::user_interface;
			aux += "\\map\\";
			aux += minimap_fn;
			strcpy(minimap_fn, aux.c_str());

			sdle::Texture m_minimap;
			m_minimap = m_texturemanager.Register(m_filemanager, minimap_fn);
			dskIngame->setMinimap(m_minimap);

			m_gui.setDesktop(dskIngame);
			m_maploaded = true;
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
			HANDLEPKT(InventoryItems, true);
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
			HANDLEPKT(ActorDisplay, true);
			default:
				_log(OPENRO__ERROR, "Unhandled packet id %d (length: %d)", pkt->getID(), pkt->size());
		}

		if (delpkt)
			delete(pkt);
		pkt = m_network.popPacket();
	}
}

void OpenRO::BeforeRun() {
	ParseClientInfo();

	InitDisplay(1024, 768, false);

	// TODO: This should be a parameter
	sdle::FTFont* lsans = new sdle::FTFont();
	FileData data = m_filemanager.getFile("font\\lsans.ttf");
	if (!lsans->openFromMemory(data.getBuffer(), data.blobSize(), 12)) {
		fprintf(stderr, "Erorr loading font 'font\\lsans.ttf'\n");
	}
	m_gui.FontManager().add("lsans.ttf", lsans);
	m_gui.setDefaultFont(lsans);

	m_texturemanager.RegisterPNG(getFileManager(), "openro\\selected.png");

	// Hide the mouse cursor
	SDL_ShowCursor(0);

	dskLogin = new DesktopLogin(this);
	dskService = new DesktopService(this);
	dskCreate = new DesktopCreate(this);
	dskChar = new DesktopChar(this);
	dskIngame = new DesktopIngame(this);
	dskLoading = new GUI::Desktop("ui\\loading.xml", *this);

	m_gui.setDesktop(dskLogin);

	FullAct ycursor;
	char xcursor[256];
	sprintf(xcursor,"sprite\\cursors");
	ycursor.Load(xcursor, *this);
	setCursor(ycursor);
}

void OpenRO::CreateCharWindow(int slot) {
	dskCreate->slot = slot;
	dskCreate->readObjects();
	m_gui.setDesktop(dskCreate);
	dskCreate->setEnabled(true);
}

void OpenRO::CreateChar(const std::string& charname, const CharAttributes& attr, int slott, unsigned short color, unsigned short style) {
	printf("Creating char in slot %d\n",slott);
	dskCreate->setEnabled(false);
	m_network.CreateChar(charname, attr, slott, color, style);
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
				strcpy(OpenRO::ConnectionIP, ip);
			}
			TiXmlElement* sclient_child_port = sclient_child->FirstChildElement("port");
			if(sclient_child_port){
				const char *port = sclient_child_port->GetText();
				OpenRO::ConnectionPort = atoi(port);
			}
			TiXmlElement* sclient_child_version = sclient_child->FirstChildElement("version");
			if(sclient_child_version){
				const char *version = sclient_child_version->GetText();
				OpenRO::ClientVersion = atoi(version);
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
	_log(OPENRO__DEBUG, "MapServer KeepAlive sent.");
}

unsigned int OpenRO::GetAccountID(){return m_serverlist->getAccountId();}
unsigned char OpenRO::GetAccountSex(){
	unsigned char male = 0x01; //m
	unsigned char female = 0x02; //f
	
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
unsigned int OpenRO::GetClientVersion(){return ClientVersion;}

void OpenRO::clickMap(int x, int y) {
	m_network.MoveCharacter(x, y);
}

void OpenRO::clickNpc(int x, int y, NpcObj* npc) {
	m_network.Talk(npc->id);
}


/* ========================================================================== *
 * Add new packets here                                                       *
 * ========================================================================== */
HNDL_IMPL(InventoryItems) {
	_log(OPENRO__DEBUG, "Received %d items in inventory",pkt->getItemCount());
}

HNDL_IMPL(ActorDisplay) {
	if (m_npc_names.find(pkt->type) != m_npc_names.end()) {
		NpcObj* npc = new NpcObj();
		npc->setMap(m_map);
		npc->open(*this, m_npc_names[pkt->type]);
		npc->setPos((float)pkt->coord_x, (float)pkt->coord_y);
		npc->id = pkt->id;
		npc->type = pkt->type;
		m_actors[pkt->id] = npc;
		return;
	}

	if (m_actors.find(pkt->id) != m_actors.end()) {
		Actor* actor = m_actors[pkt->id];
		actor->setPos((float)pkt->coord_x, (float)pkt->coord_y);
		return;
	}

	CharObj *obj = new CharObj();
	obj->open(*this, RO::J_NOVICE, RO::S_FEMALE);
	obj->setPos((float)pkt->coord_x, (float)pkt->coord_y);
	obj->setMap(m_map);

	pkt->Dump();

	m_actors[pkt->id] = obj;
}

HNDL_IMPL(CharLeaveScreen) {
	unsigned int id = pkt->getChar();
	Actor* actor;

	std::map<unsigned int, Actor*>::iterator itr = m_actors.find(id);

	if (itr != m_actors.end()) {
		actor = itr->second;
		m_actors.erase(itr);
		delete(actor);
	}
}

HNDL_IMPL(HpUpdateParty) {
	_log(OPENRO__DEBUG, "The current HP of player %d is %d/%d",pkt->getCharId(), pkt->getHp(), pkt->getMaxHp());
}

HNDL_IMPL(OtherSpeech) {
	_log(OPENRO__DEBUG, "Player %d talk: %s",pkt->getIdMes(), pkt->getText());
}

HNDL_IMPL(PlayerEquip) {
	_log(OPENRO__DEBUG, "Player %d put %d in %d (left hand %d)", pkt->getPlayer(), pkt->getID1(), pkt->getType(), pkt->getID2());
}

HNDL_IMPL(GmBroad) {
	_log(OPENRO__DEBUG, "Received GM Broadcast: %s",pkt->getText());
}

HNDL_IMPL(ServerTick) {
	unsigned int m_expected = SDL_GetTicks() - m_tickoffset;
	unsigned int m_lag = pkt->getServerTick() - m_expected;
	_log(OPENRO__DEBUG, "Received server tick: %d.\tExpected: %d.\tLag: %dms", pkt->getServerTick(), m_expected, m_lag);
}

HNDL_IMPL(MapMoveOk) {
	int x, y;

	pkt->getDest(&x, &y);
	me.setDest((float)x, (float)y);
}

HNDL_IMPL(AttackRange) {
	_log(OPENRO__TRACE, "Received attack range: %d", pkt->getRange());
}

HNDL_IMPL(GuildMessage) {
	_log(OPENRO__TRACE, "Guild Message: %s", pkt->getText());
	m_network.MapLoaded();
}

HNDL_IMPL(DisplayStat) {
	unsigned int type = pkt->getType();
	unsigned int base = pkt->getBase();
	unsigned int bonus = pkt->getBonus();

	_log(OPENRO__TRACE, "Stat %s: %d + %d", RO::dnames[type], base, bonus);
}

HNDL_IMPL(UpdateStatus) {
	unsigned short type = pkt->getType();
	unsigned int value = pkt->getValue();

	_log(OPENRO__TRACE, "Update status \"%s\", with value %d!", RO::dnames[type], value);
}

HNDL_IMPL(ServerList) {
	m_serverlist = pkt;
	m_gui.setDesktop(dskService);
	dskService->setEnabled(true);
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
	
	for(int x=0;x<count;x++){
		dskChar->addChar(pkt->getChar(x));
	}
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
	strcpy(m_mapname, pkt->getMapname());
	int i = 0;
	while (m_mapname[i] != 0) {
		if (m_mapname[i] == '.') {
			m_mapname[i] = 0;
			break;
		}
		i++;
	}
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

	m_gui.setDesktop(dskLoading);
	m_cycle = 0;
	m_maploaded = false;
	
	me.open(*this, RO::J_ALCHEMIST, RO::S_MALE);
	me.setPos(pos_x, pos_y);

	m_tickoffset = SDL_GetTicks() - pkt->getServerTick();

	_log(OPENRO__TRACE, "\tpos_x = %d\n\tpos_y = %d\n\tpos_dir = %d\n\tserver_tick = %d", pos_x, pos_y, pos_dir, server_tick);
}

HNDL_IMPL(OwnSpeech) {
	_log(OPENRO__TRACE, "OwnSpeech: %s",pkt->getText());
}

HNDL_IMPL(SkillList) {
	_log(OPENRO__TRACE, "Received skill list.");
}
