/* $Id$ */
#ifndef __RONET_PACKET_MAPMOVE_H
#define __RONET_PACKET_MAPMOVE_H

#include "ronet/packet.h"

namespace ronet {
/**
 * Packet requesting a move action to the map server
 * \ingroup ronet
 */
class RONET_DLLAPI pktMapMove: public Packet {
protected:
	virtual bool PrepareData();
	int pos_x, pos_y;

	/** Uses padding?
	 * Padding is a one-byte-data between the packet id and the data with a value of 0x44
	 */
	bool m_padding;
public:
	/** Creates a default packet */
	pktMapMove();

	/**
	 * Creates a packet with the given position
	 * @param x
	 * @param y
	 * @param padding uses a one-byte padding?
	 * @param type 0 = 4 end bits padding, 1 = 24 end bits padding
	 */
	pktMapMove(int x, int y, bool padding = false, int type = 0);

	bool Decode(ucBuffer & buf);
	void setPos(int x, int y);
};
}

#endif /* __RONET_PACKET_MAPMOVE_H */
