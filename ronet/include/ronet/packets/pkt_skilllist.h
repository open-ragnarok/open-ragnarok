/* $Id: pkt_skilllist.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_SKILLLIST_H
#define __RONET_PACKET_SKILLLIST_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktSkillList : public Packet {
	protected:
		unsigned short len_list;
	public:
		pktSkillList();
		virtual bool Decode(ucBuffer&);
	};
}

#endif /* __RONET_PACKET_SKILLLIST_H */

