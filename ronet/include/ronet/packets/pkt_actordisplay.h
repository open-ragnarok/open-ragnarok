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
#ifndef __RONET_PACKET_ACTORDISPLAY_H
#define __RONET_PACKET_ACTORDISPLAY_H

#include "ronet/packet.h"

namespace ronet {
	class RONET_DLLAPI pktActorDisplay : public Packet {
	public:
		// a4
		unsigned int id;
		// v14
		unsigned short walk_speed;
		unsigned short opt1;
		unsigned short opt2;
		unsigned short option;
		unsigned short type;
		unsigned short hair_style;
		unsigned short weapon;
		unsigned short lowhead;
		unsigned short shield;
		unsigned short tophead;
		unsigned short midhead;
		unsigned short hair_color;
		unsigned short clothes_color;
		unsigned short head_dir;
		// a4
		unsigned int guildID;
		// a2
		unsigned short emblemID;
		// v2
		unsigned short manner;
		unsigned short opt3;
		// C2
		unsigned char karma;
		unsigned char sex;
		// a3
		unsigned short coord_x, coord_y, dir;
		// C3
		unsigned char unknown1;
		unsigned char unknown2;
		unsigned char act;
		// v
		unsigned short lv;
		// ??
		unsigned char unk;

		pktActorDisplay();
		virtual bool Decode(ucBuffer&);

		void XYdecode(unsigned char buf[3]);

	};
}

#endif /* __RONET_PACKET_ACTORDISPLAY_H */
