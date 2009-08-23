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
		pktCharSelectID = 0x0066, // 3 bytes -- S 0066 <character number>.B
		pktCharCreateID = 0x0067, // 37 bytes -- S 0067 <name>.24B <str>.B <agi>.B <vit>.B <int>.B <dex>.B <luk>.B <slot>.B <hair color>.W <hair style>.W
		pktCharDeleteID = 0x0068, // 46 bytes
		pktCharDelete2ID = 0x01fb, // 56 bytes
		pktKeepAliveID = 0x0187,

		// To MAP
		pktMapLoginID = 0x0436, // (19 bytes) S 0436 <account id>.int <char id>.int <login id>.int <client tick>.unsigned int <gender>.byte

		// == INBOUND == //
		// From LOGIN
		pktServerListID = 0x0069,
		pktLoginErrorID = 0x006a, // (23 bytes) S 006A <type>.short 0x00 * 20
		pktAuthFailedID = 0x0081, // (3 bytes) S 0081 <type>.short

		// From CHAR
		pktCharListID = 0x006b,
		pktCharSelectionFailID = 0x006c, // S 006c <unk>.short
		pktCharPositionID = 0x0071, // (30 bytes) S 0071 <position>.int <mapname>.16B <ip>.int <port>.short
		pktCharCreatedID = 0x006d, // Also named "NewCharInfo" (108 or 110 bytes)
		pktCharCreateErrorID = 0x006e, // (3 bytes)
		pktCharRenameID = 0x028d,

		// From MAP
		pktMapLoginSuccessID = 0x0073, // R 0073 <server tick>.uint <coordidate>.3B ?.2B
		pktOwnSpeechID = 0x008e, // R 008e <len>.w <str>.?B
		pktMapAcctSendID = 0x0283, // (6 bytes) R 0283 <account id>.int
		pktSkillListID = 0x010f, // R 010f <len>.w {<skill ID>.w <target type>.w ?.w <lv>.w <sp>.w <range>.w <skill name>.24B <up>.B}.37B*

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
