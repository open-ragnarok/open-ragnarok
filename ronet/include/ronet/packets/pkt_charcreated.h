/* $Id$ */
#ifndef __RONET_PACKET_CHARCREATED_H
#define __RONET_PACKET_CHARCREATED_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktCharCreated : public Packet {
	protected:
		CharInformation m_char;
		unsigned char Slot;
	public:
		pktCharCreated();

		virtual bool Decode(ucBuffer&);
	};
}

#endif /* __RONET_PACKET_CHARCREATED_H */