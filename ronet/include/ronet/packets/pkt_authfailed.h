//[kR105]
#ifndef __RONET_PACKET_AUTHFAILED_H
#define __RONET_PACKET_AUTHFAILED_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktAuthFailed : public Packet {
	protected:
		short errorId;
	public:
		pktAuthFailed();
		virtual ~pktAuthFailed();

		virtual void Dump(); // For debugging purposes
		virtual bool Decode(ucBuffer&);

		short getErrorId() const;
	};
}

#endif /* __RONET_PACKET_AUTHFAILED_H */
