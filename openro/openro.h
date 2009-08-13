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

	void hndlServerList(ronet::pktServerList*);
	void hndlCharList(ronet::pktCharList*);

	void hndlLoginError(ronet::pktLoginError*);
	void hndlAuthFailed(ronet::pktAuthFailed*);

	void hndlCharCreated(ronet::pktCharCreated*);

	int m_charslot;

public:
	OpenRO();
	virtual ~OpenRO();

	void LoginScreen();
	void ProcessLogin(const std::string&, const std::string&);
	void ServiceSelect(unsigned int serviceid);
	void CreateCharWindow(int slot);
	void CreateChar(const std::string& charname, const CharAttributes&, int slott, unsigned short color, unsigned short style);
	void AddChar(CharInformation&);
	void Quit();
	void ParseClientInfo();
	void CloseSockets();
	char ConnectionIP[256];
	unsigned int ConnectionPort;
	void KeepAliveChar();
};

#endif /* __OPENRO_H */
