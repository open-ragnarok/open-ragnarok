/* $Id: pkt_updatestatus.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_UPDATESTATUS_H
#define __RONET_PACKET_UPDATESTATUS_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktUpdateStatus : public Packet {
	protected:
		unsigned short type;
		unsigned int value;
	public:
		pktUpdateStatus();
		virtual bool Decode(ucBuffer&);
		unsigned short getType();
		unsigned int getValue();
	};
}

#endif /* __RONET_PACKET_UPDATESTATUS_H */

