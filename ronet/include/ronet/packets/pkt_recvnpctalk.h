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
#ifndef __RONET_PACKET_RECVNPCTALK_H
#define __RONET_PACKET_RECVNPCTALK_H

#include "ronet/packet.h"

namespace ronet {

class RONET_DLLAPI pktRecvNpcTalk : public Packet {
protected:
	unsigned int id;
	char* message;

public:
	pktRecvNpcTalk();
	virtual ~pktRecvNpcTalk();

	const char* getMessage() const;
	unsigned int getID() const;

	virtual bool Decode(ucBuffer&);
};

}

#endif /* __RONET_PACKET_RECVNPCTALK_H */
