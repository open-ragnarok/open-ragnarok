/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "stdafx.h"

#include "ronet/packets/pkt_inventoryitemsstackable.h"


namespace ronet {

pktInventoryItemsStackable::pktInventoryItemsStackable() : Packet(pktInventoryItemsStackableID) {
	items = NULL;
	item_count = 0;
}

pktInventoryItemsStackable::~pktInventoryItemsStackable() {
	if (items != NULL)
		delete[] items;
}

short pktInventoryItemsStackable::getItemCount() const {
	return(item_count);
}
InventoryItem* pktInventoryItemsStackable::getItem(short idx) {
	if (idx < 0 || idx >= item_count)
		return(NULL);

	return(&items[idx]);
}

bool pktInventoryItemsStackable::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	buf.ignore(2);

	short pktsize;
	buf >> pktsize;
	pktsize -= 4;

	item_count = pktsize / 18; // Each item uses 18 bytes

	items = new struct InventoryItem[item_count];
	memset((void*)items, 0, sizeof(struct InventoryItem) * item_count);

	for (short x = 0; x < item_count; x++) {
		buf >> items[x].index;
		buf >> items[x].id;
		buf >> items[x].type;
		buf.ignore(3); // 3 unknown bytes
		buf >> items[x].amount;
		buf.read((unsigned char*)items[x].cards, 8);
		items[x].identified = 1;
	}

	return(true);
}


}
