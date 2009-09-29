/* $Id$ */
#ifndef __RONET_PACKET_MAPMOVEOK_H
#define __RONET_PACKET_MAPMOVEOK_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktMapMoveOk : public Packet {
	protected:
		unsigned int ticks;
		int start_x, start_y;
		int dest_x, dest_y;
	public:
		pktMapMoveOk();

		bool Decode(ucBuffer& buf);

		void getStart(int*, int*) const;
		void getDest(int*, int*) const;
	};
}

#endif /* __RONET_PACKET_LOGIN_H */
