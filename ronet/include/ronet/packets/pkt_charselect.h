/* $Id: pkt_charselect.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_CHARSELECT_H
#define __RONET_PACKET_CHARSELECT_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktCharSelect : public Packet {
	protected:
		unsigned int char_selected;

		virtual bool PrepareData();
	public:
		pktCharSelect(unsigned int acct = 0);
		virtual bool Decode(ucBuffer&);
	};
}

#endif /* __RONET_PACKET_CHARSELECT_H */

