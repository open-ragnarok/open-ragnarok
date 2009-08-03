/* $Id$ */
#ifndef __RONET_PACKET_H
#define __RONET_PACKET_H

#include "ucbuffer.h"
#include "connection.h"
#include "blob.h"
#include "structures.h"

namespace ronet {
	typedef enum {
		// == OUTBOUND == //
		// To LOGIN
		pktLoginID = 0x0064,

		// To CHAR
		pktCharLoginID = 0x0065, //  17 bytes
		pktCharSelectID = 0x0066, // 3 bytes
		pktCharCreateID = 0x0067, // 37 bytes -- S 0067 <name>.24B <str>.B <agi>.B <vit>.B <int>.B <dex>.B <luk>.B <slot>.B <hair color>.W <hair style>.W
		pktCharDeleteID = 0x0068, // 46 bytes
		pktCharDelete2ID = 0x01fb, // 56 bytes

		// To MAP
		pktMapLoginID = 0x0072, // (19 bytes) S 0072 <id1>.int <id2>.int <id3>.int <tick>.int <gender>.byte

		// == INBOUND == //
		// From LOGIN
		pktServerListID = 0x0069,
		pktLoginErrorID = 0x006a, // (23 bytes)

		// From CHAR
		pktCharListID = 0x006b,
		pktCharSelectionFailID = 0x006c, // S 006c <unk>.short
		pktCharPositionID = 0x0071, // (30 bytes) S 0071 <position>.int <mapname>.16B <ip>.int <port>.short
		pktCharCreatedID = 0x006d, // Also named "NewCharInfo" (108 or 110 bytes)
		pktCharCreateErrorID = 0x006e, // (3 bytes)
		pktKeepAliveID = 0x0187,
		pktCharRenameID = 0x028d,

		// From MAP
		pktMapLoginSuccessID = 0x0073,

		// == "WTF!?"s == //
		pktUnknown1 = 0x2974, // 74 29 00 04 05 00 d0
	} pktIds;
	class RONET_DLLAPI Packet : public DynamicBlob<unsigned char> {
	protected:
		unsigned short id;

		virtual bool PrepareData();

		bool CheckID(const ucBuffer&) const;
	public:
		Packet();
		Packet(unsigned short pktid);
		virtual ~Packet();

		Packet& operator >> (ucBuffer&);

		virtual void Dump(); // For debugging purposes
		void setSize(const unsigned long& size);
		virtual bool Decode(ucBuffer&);
		unsigned short getID() const;
	};
}

#endif /* __RONET_PACKET_H */
