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

#include "ronet/packets/generic_packets.h"

namespace ronet {
	RONET_GENERIC_ID_IMPL(RequestPlayerInfo)
	RONET_GENERIC_ID_IMPL(RequestCharacterName)
	RONET_GENERIC_TRAILING_IMPL(GetStoreInfo)
	RONET_GENERIC_IMPL(RequestIgnoreList)
	RONET_GENERIC_ID_IMPL(Take)
	RONET_GENERIC_ID_IMPL(RecvNpcInputReq)
	RONET_GENERIC_ID_IMPL(RecvNpcTalkClose)
	RONET_GENERIC_2PARAM_IMPL(StatChanged, short, short)
	
	// Guild
	RONET_GENERIC_ID_IMPL(GuildMasterMember)
	RONET_GENERIC_1PARAM_IMPL(GuildBroken, unsigned int)
	RONET_GENERIC_1PARAM_IMPL(GuildCreateResult, unsigned char)
	RONET_GENERIC_1PARAM_IMPL(GuildInviteResult, unsigned char)
	RONET_GENERIC_3PARAM_IMPL(GuildMemberOnlineStatus, unsigned int, unsigned int, unsigned int)
	RONET_GENERIC_1PARAM_IMPL(GuildAlliance, unsigned int)
	RONET_GENERIC_1PARAM_IMPL(GuildOppositionResult, unsigned char)
	RONET_GENERIC_2PARAM_IMPL(GuildUnally, unsigned int, unsigned int)
	RONET_GENERIC_3PARAM_IMPL(GuildLocation, unsigned int, unsigned short, unsigned short)

	// Pet
	RONET_GENERIC_1PARAM_IMPL(PetCaptureResult, unsigned char)
	RONET_GENERIC_2PARAM_IMPL(PetFood, unsigned char, unsigned short)
	RONET_GENERIC_3PARAM_IMPL(PetInfo2, unsigned char, unsigned int, unsigned int)
	RONET_GENERIC_2PARAM_IMPL(PetEmotion, unsigned int, unsigned int)
}