/* $Id: pkt_recvnpctalk.h 181 2009-10-07 17:16:48Z sergio $ */
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
#ifndef __RONET_PACKET_RECVNPCTALKRESPONSES_H
#define __RONET_PACKET_RECVNPCTALKRESPONSES_H

#include "ronet/packet.h"

namespace ronet {

class RONET_DLLAPI pktRecvNpcTalkResponses : public Packet {
protected:
	unsigned int npc;
	char* message;

	std::vector<std::string> lines;

	void DecodeMessage();

public:
	pktRecvNpcTalkResponses();
	virtual ~pktRecvNpcTalkResponses();

	const char* getMessage() const;
	unsigned int getNPC() const;
	unsigned int getLineCount() const;
	const std::string& getLine(unsigned int i) const;

	virtual bool Decode(ucBuffer&);
};

}

#endif /* __RONET_PACKET_RECVNPCTALKRESPONSES_H */
