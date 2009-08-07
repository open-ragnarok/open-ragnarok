//[kR105]
#ifndef __RONET_PACKET_LOGINERROR_H
#define __RONET_PACKET_LOGINERROR_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktLoginError : public Packet {
	protected:
		short errorId;
	public:
		pktLoginError();
		virtual ~pktLoginError();

		virtual void Dump(); // For debugging purposes
		virtual bool Decode(ucBuffer&);

		short getErrorId() const;
	};
}

#endif /* __RONET_PACKET_LOGINERROR_H */
