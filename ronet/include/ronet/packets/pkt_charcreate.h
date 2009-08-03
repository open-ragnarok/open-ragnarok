/* $Id$ */
#ifndef __RONET_PACKET_CHARCREATE_H
#define __RONET_PACKET_CHARCREATE_H

#include "ronet/packet.h"

namespace ronet {
	// S 0067 <name>.24B <str>.B <agi>.B <vit>.B <int>.B <dex>.B <luk>.B <slot>.B <hair color>.W <hair style>.W
	class RONET_DLLAPI pktCharCreate : public Packet {
	protected:
		std::string charname;
		CharAttributes attributes;
		//unsigned char Str, Agi, Vit, Int, Dex, Luk;
		unsigned char Slot;
		unsigned short color;
		unsigned short style;

		virtual bool PrepareData();
	public:
		pktCharCreate();
		pktCharCreate(const std::string& charname, const CharAttributes&, unsigned char slot, unsigned short color, unsigned short style);
	};
}

#endif /* __RONET_PACKET_LOGIN_H */
