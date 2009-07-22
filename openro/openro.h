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

	int m_charslot;

public:
	OpenRO();
	virtual ~OpenRO();

	void LoginScreen();
	void ProcessLogin(const std::string&, const std::string&);
	void ServiceSelect(unsigned int serviceid);
	void CreateCharWindow(unsigned int& slot);
	void CreateChar(const std::string& charname, const CharAttributes&, unsigned short color, unsigned short style);
	void AddChar(CharInformation&);
	void Quit();
};

#endif /* __OPENRO_H */
