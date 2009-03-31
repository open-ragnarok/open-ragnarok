/* $Id$ */
#ifndef __RONET_PACKET_CHARLIST_H
#define __RONET_PACKET_CHARLIST_H

#include "ronet/packet.h"
// no chars
// 6b 00 04 00 74 29 00 04 05 00 d0                  k...t).....

/*
Example
0000  6b 00 6e 00 f5 a2 03 00 00 00 00 00 00 00 00 00   k.n.............
0010  00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00   ................
0020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 2a 00   ..............*.
0030  28 00 0b 00 0b 00 96 00 00 00 02 00 00 00 01 00   (...............
0040  00 00 00 00 00 00 00 00 00 00 00 00 00 00 49 63   ..............Ic
0050  65 54 65 6d 70 65 73 74 00 00 00 00 00 00 00 00   eTempest........
0060  00 00 00 00 00 00 05 05 05 05 05 05 00 39 74 29   .............9t)
0070  00 04 05 00 d0                                    .....
*/

namespace ronet {
	class RONET_DLLAPI pktCharList : public Packet {
	protected:
		int m_count;

		CharInformation *m_chars;
	public:
		pktCharList();
		virtual ~pktCharList();

		virtual bool Decode(ucBuffer&);
		int getCount() const;
		const CharInformation& getChar(int i) const;
	};

}

#endif /* __RONET_PACKET_CHARLIST_H */
