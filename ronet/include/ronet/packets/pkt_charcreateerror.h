#ifndef __RONET__CHARCREATEERROR_H
#define __RONET__CHARCREATEERROR_H

#include "ronet/packet.h"

namespace ronet {

// S 0064 <version>.l <account name>.24B <password>.24B <version2>.B
// S 01dd <version>.l <account name>.24B <md5 binary>.16B <version2>.B
// S 0277 <version>.l <account name>.24B <password>.24B <junk?>.29B <version2>.B
// S 02b0 <version>.l <account name>.24B <password>.24B <junk?>.30B <version2>.B

	class RONET_DLLAPI pktCharCreateError : public Packet {
	protected:
		char errid;
	public:
		pktCharCreateError();

		const char& getId() const;
		virtual bool Decode(ucBuffer&);
	};
}

#endif /* __RONET__CHARCREATEERROR_H */
