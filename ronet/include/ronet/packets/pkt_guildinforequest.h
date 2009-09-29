/* $Id$ */
#ifndef __RONET_PACKET_GUILDINFOREQUEST_H
#define __RONET_PACKET_GUILDINFOREQUEST_H

#include "ronet/packet.h"

namespace ronet {

// S 014d 
	class RONET_DLLAPI pktGuildInfoRequest : public Packet {
	protected:
	public:
		pktGuildInfoRequest();
	};
}

#endif /* __RONET_PACKET_LOGIN_H */
