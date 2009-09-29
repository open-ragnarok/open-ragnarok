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

	/**
	 * Dumps an hex buffer to the standard output stream
	 * @param buf the memory array to dump
	 * @param buflen the length of the data to dump
	 */
	void hexdump(const unsigned char* buf, unsigned int buflen);

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
	
	public:
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

		bool GameLogin(const std::string& name, const std::string& pass);
		bool CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex);
		bool CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style);
		void KeepAliveChar(unsigned int acc_id);

		/** Selects a character */
		void CharSelect(unsigned int slot);

		/** Login to the map serve */
		bool MapLogin(int acctid, int sid1, int sid2, unsigned int tick, int sex);

		bool RequestGuiildInfo();
		bool MapLoaded();


		/** Moves the character
		 *
		 * @return true on success. False if no success (like: not connected to map server)
		 */
		bool MoveCharacter(int x, int y);
	};
}

#endif /* __RONET_H */
