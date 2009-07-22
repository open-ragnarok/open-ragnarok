/* $Id$ */
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
	class RONET_DLLAPI RONet {
	protected:
		TcpConnection m_login;
		TcpConnection m_char;
		TcpConnection m_map;

		PacketFactory m_packetfactory;
	
	public:
		Packet* popPacket();
		int packetCount() const;
		bool Process();

		TcpConnection& getLogin();
		TcpConnection& getChar();
		TcpConnection& getMap();

		bool GameLogin(const std::string& name, const std::string& pass);
		bool CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex);
		bool CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style);
	};
}

#endif /* __RONET_H */
