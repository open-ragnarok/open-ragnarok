/* $Id: pkt_ownspeech.h 73 2009-08-03 16:45:59Z sergio $ */
#ifndef __RONET_PACKET_OWNSPEECH_H
#define __RONET_PACKET_OWNSPEECH_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktOwnSpeech : public Packet {
	protected:
		unsigned short len_mes;
		char text[256];
	public:
		pktOwnSpeech();
		virtual bool Decode(ucBuffer&);
		char *getText();
	};
}

#endif /* __RONET_PACKET_OWNSPEECH_H */

