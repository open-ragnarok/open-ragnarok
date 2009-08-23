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
		short pos_x;
		short pos_y; 
		short pos_dir;
	public:
		pktMapLoginSuccess();

		bool Decode(ucBuffer& buf);

		short getPosX() const;

		short getPosY() const;

		short getPosDir() const;

		unsigned int getServerTick() const;

		void pktMapLoginSuccess::XYdecode(unsigned char buf[3]);

	};
}

#endif /* __RONET_PACKET_LOGINSUCCESS_H */
