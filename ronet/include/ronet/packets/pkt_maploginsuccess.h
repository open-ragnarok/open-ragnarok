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
class pktMapLoginSuccess: public Packet {
protected:
	unsigned int unk1;
	unsigned short unk2;
	char pos1;
	char pos2;
	char pos3;
public:
	pktMapLoginSuccess();

	bool Decode(ucBuffer& buf);

	char getPos1() const;

	char getPos2() const;

	char getPos3() const;

	unsigned int getUnk1() const;

	unsigned short getUnk2() const;

};
}

#endif /* __RONET_PACKET_LOGINSUCCESS_H */
