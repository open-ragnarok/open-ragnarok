/* $Id: pkt_updatestatus.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_ATTACKRANGE_H
#define __RONET_PACKET_ATTACKRANGE_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktAttackRange : public Packet {
	protected:
		unsigned short range;
	public:
		pktAttackRange();
		virtual bool Decode(ucBuffer&);
		unsigned short getRange();
	};
}

#endif /* __RONET_PACKET_ATTACKRANGE_H */

