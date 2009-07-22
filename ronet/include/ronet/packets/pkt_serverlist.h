/* $Id$ */
#ifndef __RONET_PACKET_SERVERLIST_H
#define __RONET_PACKET_SERVERLIST_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktServerList : public Packet {
	protected:
		typedef struct {
			unsigned int ip;
			unsigned short port;
			char name[20];
			unsigned short users;
			unsigned short maintenance;
			unsigned short New; // ??
		} ServerInfo;

		int servercount;
		ServerInfo* servers;
		unsigned int account_id;
		unsigned int sessionid1, sessionid2;
		unsigned char sex;
	public:
		pktServerList();
		virtual ~pktServerList();

		virtual void Dump(); // For debugging purposes
		virtual bool Decode(ucBuffer&);

		const ServerInfo& getInfo(const int&) const;
		unsigned int getServerCount() const;
		unsigned int getAccountId() const;
		unsigned int getSessionId1() const;
		unsigned int getSessionId2() const;
		unsigned char getSex() const;
	};
}

#endif /* __RONET_PACKET_SERVERLIST_H */