/* $Id$ */
#ifndef __RONET_PACKET_CHARLOGIN_H
#define __RONET_PACKET_CHARLOGIN_H

#include "ronet/packet.h"

namespace ronet {

// S 0065 <account id>.i <session id 1>.i <session id 2>.i <unk>.s <sex>.B

	class RONET_DLLAPI pktCharLogin : public Packet {
	protected:
		unsigned int accountid;
		unsigned int sessionid1, sessionid2;
		unsigned char sex;

		virtual bool PrepareData();
	public:
		pktCharLogin(unsigned int acct, unsigned int sid1, unsigned int sid2, unsigned char s);
	};
}

#endif /* __RONET_PACKET_LOGIN_H */
