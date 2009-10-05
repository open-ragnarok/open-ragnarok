/* $Id: pkt_hpupdateparty.cc 147 2009-09-30 12:13:45Z sergio $ */
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

#include "ronet/packets/pkt_hpupdateparty.h"

ronet::pktHpUpdateParty::pktHpUpdateParty() : Packet(pktHpUpdatePartyID) {
}

bool ronet::pktHpUpdateParty::Decode(ucBuffer& buf) {
	// Sanity Check
	if (!CheckID(buf))
		return(false);

	if (buf.dataSize() < 10)
		return(false);

	buf.ignore(2);
	buf >> char_id;
	buf >> hp;
	buf >> max_hp;

	return(true);
}

unsigned int ronet::pktHpUpdateParty::getCharId() const {
	return char_id;
}

unsigned short ronet::pktHpUpdateParty::getHp() const {
	return hp;
}

unsigned short ronet::pktHpUpdateParty::getMaxHp() const {
	return max_hp;
}