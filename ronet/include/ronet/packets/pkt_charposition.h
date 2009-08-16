/* $Id$ */
#ifndef __RONET_PACKET_CHARPOSITION_H
#define __RONET_PACKET_CHARPOSITION_H

#include "ronet/packet.h"

namespace ronet {
/**
 * Packet describing the player position, map and ip/port where the character is located at.
 */
class RONET_DLLAPI pktCharPosition: public Packet {
protected:
	unsigned int position;
	char mapname[16];
	unsigned int ip;
	unsigned short port;
public:
	pktCharPosition();
	bool Decode(ucBuffer & buf);
	unsigned int getIp() const;
	const char* getMapname() const;
	unsigned short getPort() const;
	unsigned int getPosition() const;
};
}

#endif /* __RONET_PACKET_CHARPOSITION_H */
