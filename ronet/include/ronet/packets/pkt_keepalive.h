/* $Id$ */
#ifndef __RONET_PACKET_KEEPALIVE_H
#define __RONET_PACKET_KEEPALIVE_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktKeepAlive : public Packet {
	protected:
		unsigned int account_id;

		virtual bool PrepareData();
	public:
		pktKeepAlive(unsigned int acct = 0);
		virtual bool Decode(ucBuffer&);
		unsigned int getAccountId() const;
	};
}

#endif /* __RONET_PACKET_KEEPALIVE_H */

