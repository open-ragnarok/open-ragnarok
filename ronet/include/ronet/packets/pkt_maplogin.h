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
#ifndef __RONET_PACKET_MAPLOGIN_H
#define __RONET_PACKET_MAPLOGIN_H

#include "ronet/packet.h"

namespace ronet {
/**
 * Packet describing the player position, map and ip/port where the character is located at.
 */
class RONET_DLLAPI pktMapLogin : public Packet {
protected:
	virtual bool PrepareData();
	int account_id;
	int char_id;
	int login_id1;
	unsigned int client_tick;
	char gender;
public:
	pktMapLogin();
	pktMapLogin(int, int, int, unsigned int, char);
	bool Decode(ucBuffer & buf);
	void setGender(char gender);

	void setSession_id1(int account_id);

	void setSession_id2(int char_id);

	void setSession_id3(int login_id1);

	void setTick(unsigned int client_tick);

};

class RONET_DLLAPI pktMapLogin23 : public pktMapLogin {
public:
	pktMapLogin23();
	pktMapLogin23(int, int, int, unsigned int, char);
};

}

#endif /* __RONET_PACKET_MAPLOGIN_H */
