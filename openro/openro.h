#ifndef __OPENRO_H
#define __OPENRO_H

// Forward definition
class OpenRO;

#include "roengine/ro_engine.h"
#include "gui/desktops.h"

#include "states.h"

#include "ronet/ronet.h"

#include <string>

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

	// Useful Packets
	ronet::pktServerList* m_serverlist;

	//Add new packets here
	void hndlAttackRange(ronet::pktAttackRange*);
	void hndlGuildMessage(ronet::pktGuildMessage*);
	void hndlDisplayStat(ronet::pktDisplayStat*);
	void hndlUpdateStatus(ronet::pktUpdateStatus*);
	void hndlServerList(ronet::pktServerList*);
	void hndlCharList(ronet::pktCharList*);
	void hndlLoginError(ronet::pktLoginError*);
	void hndlAuthFailed(ronet::pktAuthFailed*);
	void hndlCharCreated(ronet::pktCharCreated*);
	void hndlCharPosition(ronet::pktCharPosition*);
	void hndlMapAcctSend(ronet::pktMapAcctSend*);
	void hndlMapLoginSuccess(ronet::pktMapLoginSuccess*); 
	void hndlOwnSpeech(ronet::pktOwnSpeech*);
	void hndlSkillList(ronet::pktSkillList*);

	int m_charslot;

	/** IP address to connect */
	char ConnectionIP[256];

	/** Port to connect */
	unsigned int ConnectionPort;

	/** Parses the client information on the given xml file */
	void ParseClientInfo(const std::string& name = "sclientinfo.xml");

public:
	OpenRO();
	virtual ~OpenRO();

	void LoginScreen();
	void CharSelectScreen();
	void ProcessLogin(const std::string&, const std::string&);
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

	unsigned int GetAccountID();
	unsigned char GetAccountSex();

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
