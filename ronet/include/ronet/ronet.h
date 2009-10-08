/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#ifndef __RONET_H
#define __RONET_H

#include "ronet_import.h"
#include "structures.h"
#include "net_tools.h"
#include "tcp_connection.h"
#include "buffer.h"
#include "packet_factory.h"

#include "packets/packets.h"

namespace ronet {

/** Configures log output when using LogSys */
void ConfigureLog();

/** 
 * The RO Network manager
 */
class RONET_DLLAPI RONet {
protected:
	/** Connection to the login server. The 1st server to connect. */
	TcpConnection m_login;

	/** Connection to the char server. The 2nd server to connect. */
	TcpConnection m_char;

	/** Connection to the map server. The 3rd server to connect. */
	TcpConnection m_map;

	/** Generates the packets for the application. */
	PacketFactory m_packetfactory;

	/** The current used packet version (default: 22) */
	unsigned int m_packetver;

public:
	RONet();

	Packet* popPacket();

	/** Return how many packets we have in the receive queue */
	int packetCount() const;

	/** Process the inbound and outbound data and connections */
	bool Process();

	/** Retrieves the connection to the login server */
	TcpConnection& getLogin();

	/** Retrieves the connection to the char server */
	TcpConnection& getChar();

	/** Retrieves the connection to the map server */
	TcpConnection& getMap();

	unsigned int getPacketVer() const;
	void setPacketVer(unsigned int);

	bool GameLogin(const std::string& name, const std::string& pass, unsigned int version);
	bool CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex);
	bool CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style);
	void KeepAliveChar(unsigned int acc_id);

	/** Selects a character */
	void CharSelect(unsigned int slot);

	/** Login to the map serve */
	bool MapLogin(int acctid, int sid1, int sid2, unsigned int tick, int sex);

	bool RequestGuiildInfo();
	bool MapLoaded();

	bool Talk(unsigned int npc_id);

	bool NPCNext(unsigned int id);
	bool NPCCancel(unsigned int id);
	bool NPCResponse(unsigned int id, unsigned char);


	/** Moves the character
	 *
	 * @return true on success. False if no success (like: not connected to map server)
	 */
	bool MoveCharacter(int x, int y);

	void KeepAliveMap(unsigned int acc_id);
};
}

#endif /* __RONET_H */
