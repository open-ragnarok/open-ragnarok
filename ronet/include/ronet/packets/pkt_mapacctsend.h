/* $Id: pkt_keepalive.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_MAPACCTSEND_H
#define __RONET_PACKET_MAPACCTSEND_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktMapAcctSend : public Packet {
	protected:
		int account_id;
	public:
		pktMapAcctSend(int acct = 0);
		virtual bool Decode(ucBuffer&);
		int getAccountId() const;
	};
}

#endif /* __RONET_PACKET_MAPACCTSEND_H */

