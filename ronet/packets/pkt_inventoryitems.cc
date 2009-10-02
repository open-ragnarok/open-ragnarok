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

#include "ronet/packets/pkt_inventoryitems.h"


namespace ronet {

pktInventoryItems::pktInventoryItems() : Packet(pktInventoryItemsID) {
	items = NULL;
	item_count = 0;
}

pktInventoryItems::~pktInventoryItems() {
	if (items != NULL)
		delete[] items;
}

short pktInventoryItems::getItemCount() const {
	return(item_count);
}
InventoryItem* pktInventoryItems::getItem(short idx) {
	if (idx < 0 || idx >= item_count)
		return(NULL);

	return(&items[idx]);
}

bool pktInventoryItems::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		_log(RONET__ERROR, "Wrong packet id! Received: %04x, Expected: %04x", buf_id, id);
		return(false);
	}

	buf.ignore(2);

	short pktsize;
	buf >> pktsize;
	pktsize -= 4;

	item_count = pktsize / 20; // Each item uses 20 bytes

	items = new struct InventoryItem[item_count];

	for (short x = 0; x < item_count; x++) {
		buf >> items[x].index;
		buf >> items[x].id;
		buf >> items[x].type;
		buf >> items[x].identified;
		buf >> items[x].type_equip;
		buf >> items[x].equipped;
		buf >> items[x].broken;
		buf >> items[x].upgrade;
		buf.read((unsigned char*)items[x].cards, 8);
	}

	return(true);
}


}
