/* $Id: pkt_ownspeech.h */
#ifndef __RONET_PACKET_GUILDMESSAGE_H
#define __RONET_PACKET_GUILDMESSAGE_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktGuildMessage : public Packet {
	protected:
		unsigned short len_mes;
		char text[256];
	public:
		pktGuildMessage();
		virtual bool Decode(ucBuffer&);
		char *getText();
	};
}

#endif /* __RONET_PACKET_GUILDMESSAGE_H */

