/* $Id$ */
/*
 * pkt_maploginsuccess.h
 *
 *  Created on: 03/08/2009
 */

#ifndef __RONET_PACKET_LOGINSUCCESS_H
#define __RONET_PACKET_LOGINSUCCESS_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktMapLoginSuccess : public Packet {
	protected:
		unsigned int server_tick;
		unsigned char pos_x;
		unsigned char pos_y; 
		unsigned char pos_dir;
	public:
		pktMapLoginSuccess();

		bool Decode(ucBuffer& buf);

		short getPosX() const;

		short getPosY() const;

		unsigned char getPosDir() const;

		unsigned int getServerTick() const;

	};
}

#endif /* __RONET_PACKET_LOGINSUCCESS_H */
