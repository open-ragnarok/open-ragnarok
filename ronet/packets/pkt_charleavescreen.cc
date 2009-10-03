/* $Id: pkt_charleavescreen.cc 149 2009-09-30 18:57:25Z sergio $ */
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

#include "ronet/packets/pkt_maploaded.h"

namespace ronet {

pktCharLeaveScreen::pktCharLeaveScreen() : Packet(pktCharLeaveScreenID) {
	char_id = 0;
	char_type = 0;
}

bool pktCharLeaveScreen::Decode(ucBuffer& buf) {
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! Expected %04x, received %04x.\n", id, buf_id);
		return(false);
	}

	// Packet size is 12...
	if (buf.dataSize() < 12)
		return(false);

	buf.ignore(2); // id
	buf >> char_id;
	buf >> char_type;

	return(true);
}

unsigned int pktCharLeaveScreen::getChar() const {
	return(char_id);
}

unsigned char pktCharLeaveScreen::getType() const {
	return(char_type);
}

}