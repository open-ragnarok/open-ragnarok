/* $Id$ */
#ifndef __RONET_PACKET_MAPLOGIN_H
#define __RONET_PACKET_MAPLOGIN_H

#include "ronet/packet.h"

namespace ronet {
/**
 * Packet describing the player position, map and ip/port where the character is located at.
 */
class RONET_DLLAPI pktMapLogin: public Packet {
protected:
	virtual bool PrepareData();
	int account_id;
	int char_id;
	int login_id1;
	unsigned int client_tick;
	char gender;
public:
	pktMapLogin();
	pktMapLogin(int, int, int, unsigned int, char);
	bool Decode(ucBuffer & buf);
	void setGender(char gender);

	void setSession_id1(int account_id);

	void setSession_id2(int char_id);

	void setSession_id3(int login_id1);

	void setTick(unsigned int client_tick);

};
}

#endif /* __RONET_PACKET_MAPLOGIN_H */
