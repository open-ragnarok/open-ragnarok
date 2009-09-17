/* $Id: pkt_updatestatus.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_DISPLAYSTAT_H
#define __RONET_PACKET_DISPLAYSTAT_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktDisplayStat : public Packet {
	protected:
		unsigned int type;
		unsigned int base;
		unsigned int bonus;
	public:
		pktDisplayStat();
		virtual bool Decode(ucBuffer&);
		unsigned int getType();
		unsigned int getBase();
		unsigned int getBonus();
	};
}

#endif /* __RONET_PACKET_DISPLAYSTAT_H */

