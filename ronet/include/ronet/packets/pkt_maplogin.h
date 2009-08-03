/* $Id$ */
#ifndef __RONET_PACKET_MAPLOGIN_H
#define __RONET_PACKET_MAPLOGIN_H

#include "ronet/packet.h"

namespace ronet {
/**
 * Packet describing the player position, map and ip/port where the character is located at.
 */
class pktMapLogin: public Packet {
protected:
	unsigned int session_id1;
	unsigned int session_id2;
	unsigned int session_id3;
	unsigned int tick;
	char gender;
public:
	pktMapLogin();
	pktMapLogin(unsigned int, unsigned int, unsigned int, unsigned int, char);
	bool Decode(ucBuffer & buf);
	void setGender(char gender);

	void setSession_id1(unsigned int session_id1);

	void setSession_id2(unsigned int session_id2);

	void setSession_id3(unsigned int session_id3);

	void setTick(unsigned int tick);

};
}

#endif /* __RONET_PACKET_MAPLOGIN_H */
