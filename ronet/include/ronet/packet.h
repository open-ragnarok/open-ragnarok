/* $Id$ */
#ifndef __RONET_PACKET_H
#define __RONET_PACKET_H

#include "buffer.h"
#include "connection.h"
#include "blob.h"

namespace ronet {
	class RONET_DLLAPI Packet : public DynamicBlob<unsigned char> {
	protected:
		unsigned short id;

		virtual bool PrepareData();
	public:
		Packet();
		Packet(unsigned short pktid);
		virtual ~Packet();

		Packet& operator >> (ucBuffer&);

		virtual void Dump(); // For debugging purposes
		void setSize(const unsigned long& size);
		virtual bool Decode(ucBuffer&);
	};
}

#endif /* __RONET_PACKET_H */