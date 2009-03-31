/* $Id$ */
#ifndef __RONET_PACKET_LOGIN_H
#define __RONET_PACKET_LOGIN_H

#include "ronet/packet.h"

namespace ronet {

// S 0064 <version>.l <account name>.24B <password>.24B <version2>.B
// S 01dd <version>.l <account name>.24B <md5 binary>.16B <version2>.B
// S 0277 <version>.l <account name>.24B <password>.24B <junk?>.29B <version2>.B
// S 02b0 <version>.l <account name>.24B <password>.24B <junk?>.30B <version2>.B

	class RONET_DLLAPI pktLogin : public Packet {
	protected:
		unsigned int version;
		std::string username, password;
		unsigned char version2;


		virtual bool PrepareData();
	public:
		pktLogin();
		pktLogin(const std::string& user, const std::string& pass);
	};
}

#endif /* __RONET_PACKET_LOGIN_H */