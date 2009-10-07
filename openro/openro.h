#ifndef __OPENRO_H
#define __OPENRO_H

// Forward definition
class OpenRO;

#include "roengine/ro_engine.h"
#include "gui/desktops.h"

#include "states.h"

#include "ronet/ronet.h"

#include <string>

/** Handler declaration for network packets */
#define HNDL_DECL(x) void hndl ##x (ronet::pkt ##x *)

/** Handler implementation (function) for network packets */
#define HNDL_IMPL(x) void OpenRO::hndl ##x (ronet::pkt ##x * pkt)

class OpenRO : public ROEngine {
protected:
	/** Current client state */
	ROState m_state;
	ronet::RONet m_network;

	virtual void BeforeRun();
	virtual void AfterDraw();

	// TODO: Remove this in the near future (before Alpha)
	virtual void HandleKeyboard();

	// Desktops
	DesktopLogin* dskLogin;
	DesktopService* dskService;
	DesktopCreate* dskCreate;
	DesktopChar* dskChar;
	DesktopIngame* dskIngame;
	GUI::Desktop* dskLoading;

	// Useful Packets
	ronet::pktServerList* m_serverlist;

	//Add new packets here
	HNDL_DECL(InventoryItems);
	HNDL_DECL(HpUpdateParty);
	HNDL_DECL(OtherSpeech);
	HNDL_DECL(PlayerEquip);
	HNDL_DECL(CharLeaveScreen);
	HNDL_DECL(GmBroad);
	HNDL_DECL(ServerTick);
	HNDL_DECL(AttackRange);
	HNDL_DECL(GuildMessage);
	HNDL_DECL(DisplayStat);
	HNDL_DECL(UpdateStatus);
	HNDL_DECL(ServerList);
	HNDL_DECL(CharList);
	HNDL_DECL(LoginError);
	HNDL_DECL(AuthFailed);
	HNDL_DECL(CharCreated);
	HNDL_DECL(CharPosition);
	HNDL_DECL(MapAcctSend);
	HNDL_DECL(MapLoginSuccess);
	HNDL_DECL(OwnSpeech);
	HNDL_DECL(SkillList);
	HNDL_DECL(MapMoveOk);
	HNDL_DECL(ActorDisplay);

	int m_charslot;

	/** IP address to connect */
	char ConnectionIP[256];

	/** Port to connect */
	unsigned int ConnectionPort;

	/** Client Version taken from (s)clientinfo.xml */
	unsigned int ClientVersion;

	/** Parses the client information on the given xml file */
	void ParseClientInfo(const std::string& name = "sclientinfo.xml");

	/** Store the name of the map load */
	char m_mapname[256];
	bool m_maploaded;
	int m_cycle;

public:
	OpenRO();
	virtual ~OpenRO();

	void LoginScreen();
	void CharSelectScreen();
	void ProcessLogin(const std::string&, const std::string&, unsigned int version);
	void ServiceSelect(unsigned int serviceid);
	void CharSelect(unsigned int slot);
	void CreateCharWindow(int slot);
	void CreateChar(const std::string& charname, const CharAttributes&, int slott, unsigned short color, unsigned short style);
	void AddChar(CharInformation&);

	/** Loads a map and sets it as default */
	void LoadMap(const char* map);

	/** Quits the game */
	void Quit();

	/** Closes all connections */
	void CloseSockets();

	void KeepAliveChar();

	void KeepAliveMap();

	unsigned int GetAccountID();
	unsigned char GetAccountSex();
	unsigned int GetClientVersion();

	/** Event received from ROEngine */
	virtual void clickMap(int x, int y);

	/** Event received from ROEngine */
	//virtual void clickMob(int x, int y);

	/** Event received from ROEngine */
	//virtual void clickItem(int x, int y);

	/** Event received from ROEngine */
	//virtual void clickPortal(int x, int y);
};

#endif /* __OPENRO_H */
