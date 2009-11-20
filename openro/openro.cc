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
	m_network.getLogin().Connect(m_connectionip, m_connectionport);
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
		m_gui.setDesktop(dskLoading);

		if (m_cycle <= 2)
			m_cycle++;
		else {
			if (m_map != NULL) {
				delete(m_map);
				m_map = NULL;
			}

			m_map = RswObject::open(*this, m_mapname);
			if (m_map == NULL) {
				_log(OPENRO__ERROR, "Error loading map %s!", m_mapname);
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
			HANDLEPKT(RecvNpcTalk, true);
			HANDLEPKT(RecvNpcTalkNext, true);
			HANDLEPKT(RecvNpcTalkClose, true);
			HANDLEPKT(MapChange, true);
			HANDLEPKT(RecvNpcInputReq, true);
			HANDLEPKT(RecvNpcTalkResponses, true);
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

	SDL_WM_SetCaption("Open Ragnarok - www.open-ragnarok.org", "OpenRO");

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

unsigned int OpenRO::GetClientVersion(){
	return(m_clientversion);
}

void OpenRO::clickMap(int x, int y) {
	m_network.MoveCharacter(x, y);
}

void OpenRO::clickNpc(int x, int y, NpcObj* npc) {
	m_network.Talk(npc->id);
}

void OpenRO::clickMob(int x, int y, MobObj* mob) {
	m_network.MoveCharacter(x, y); // Need this?
	m_network.sendAction(mob->id, 7);
	_log(OPENRO__DEBUG, "Attacking mob %d", mob->id);
}

void OpenRO::clickPortal(int x, int y, NpcObj* npc) {
	m_network.RequestPlayerInfo(npc->id);
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

	int i = 0;
	while (mapname[i] != 0) {
		if (mapname[i] == '.') {
			mapname[i] = 0;
			break;
		}
		i++;
	}

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
}

void OpenRO::HandleActorInfo(const struct ActorInfo* info) {
	Actor *actor = NULL;

	if (m_actors.find(info->id) != m_actors.end()) {
		// Already on the database...
		actor = m_actors[info->id];
	}
	else if (m_npc_names.find(info->type) != m_npc_names.end()) {
		// Actor is a NPC
		NpcObj* npc = new NpcObj();
		actor = npc;
		npc->open(*this, m_npc_names[info->type]);
	}
	else if (m_job_names.find(info->type) != m_job_names.end()) {
		// Actor is a player
		CharObj *obj = new CharObj();
		actor = obj;
		obj->open(*this, (ro::CJob)info->type, (ro::CSex)info->sex);
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
			printf("DAMAGE: %d->%d. Amount: %d\n", pkt->sourceID, pkt->targetID, pkt->damage);
			break;
		case 1:
			_log(OPENRO__DEBUG, "User %d get item %d", pkt->sourceID, pkt->targetID);
			break;
		case 2:
			_log(OPENRO__DEBUG, "User %d sits down", pkt->sourceID);
			break;
		case 3:
			_log(OPENRO__DEBUG, "Damage? %d->%d, damage: %d", pkt->sourceID, pkt->targetID, pkt->damage);
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


HNDL_IMPL(InventoryItems) {
	_log(OPENRO__DEBUG, "Received %d items in inventory",pkt->getItemCount());
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
	//_log(OPENRO__DEBUG, "Received server tick: %d.\tExpected: %d.\tLag: %dms", pkt->getServerTick(), m_expected, m_lag);
	dskIngame->m_lag = m_lag;
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
	//m_network.MapLoaded();
}

HNDL_IMPL(DisplayStat) {
	unsigned int type = pkt->getType();
	unsigned int base = pkt->getBase();
	unsigned int bonus = pkt->getBonus();

	_log(OPENRO__TRACE, "Stat %s: %d + %d", ro::dnames[type], base, bonus);
}

HNDL_IMPL(UpdateStatus) {
	unsigned short type = pkt->getType();
	unsigned int value = pkt->getValue();

	switch(type) {
		case 5: // HP
			dskIngame->setHP(value);
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
		case 20: // Zeny
			dskIngame->setZeny(value);
			break;
		case 24: // Weight
			dskIngame->setWeight((float)value/10.0f);
			break;
		case 25: // Max Weight
			dskIngame->setMaxWeight((float)value/10.0f);
			break;
		default:
			_log(OPENRO__TRACE, "Unhandled update status \"%s\" (%d), with value %d", ro::dnames[type], type, value);
	}
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
	LoadMap(pkt->getMapname());
}

HNDL_IMPL(MapChange) {
	me.setPos((float)pkt->getPosX(), (float)pkt->getPosY());
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
	
	me.open(*this, ro::J_ALCHEMIST, ro::S_MALE);
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
