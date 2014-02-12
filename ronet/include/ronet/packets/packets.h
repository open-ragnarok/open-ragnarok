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
#ifndef __RONET_PACKETS_PACKETS_H
#define __RONET_PACKETS_PACKETS_H

//Add new packets here
#include "generic_packets.h"
#include "pkt_charcreate.h"
#include "pkt_charcreated.h"
#include "pkt_charcreateerror.h"
#include "pkt_charlist.h"
#include "pkt_charlogin.h"
#include "pkt_keepalive.h"
#include "pkt_login.h"
#include "pkt_login2.h"
#include "pkt_serverlist.h"
#include "pkt_loginerror.h"			//[kR105]
#include "pkt_authfailed.h"			//[kR105]
#include "pkt_charselect.h"			//[kR105]
#include "pkt_charposition.h"
#include "pkt_maplogin.h"
#include "pkt_mapacctsend.h"		//[kR105] 
#include "pkt_maploginsuccess.h"
#include "pkt_ownspeech.h"			//[kR105]
#include "pkt_skilllist.h"			//[kR105]
#include "pkt_updatestatus.h"		//[kR105]
#include "pkt_displaystat.h"		//[kR105]
#include "pkt_guildmessage.h"		//[kR105]
#include "pkt_attackrange.h"		//[kR105]
#include "pkt_mapmove.h"
#include "pkt_guildinforequest.h"
#include "pkt_maploaded.h"
#include "pkt_mapmoveok.h"
#include "pkt_keepalivemap.h"		//[kR105]
#include "pkt_servertick.h"			//[kR105]
#include "pkt_playerequip.h"
#include "pkt_itemgained.h"
#include "pkt_inventoryitems.h"
#include "pkt_inventoryitemsstackable.h"
//#include "pkt_statinfo.h"
//#include "pkt_statinfo2.h"
#include "pkt_hotkeylist.h"
#include "pkt_zenyexp.h"
#include "pkt_statsinfo.h"
#include "pkt_gmbroad.h"			//[kR105]
#include "pkt_charleavescreen.h"	//[kR105]
#include "pkt_otherspeech.h"		//[kR105]
#include "pkt_hpupdateparty.h"		//[kR105]
#include "pkt_actordisplay.h"
#include "pkt_actorspawn.h"
#include "pkt_actorwalking.h"
#include "pkt_talk.h"
#include "pkt_recvnpctalk.h"
#include "pkt_recvnpcimage2.h"
#include "pkt_recvnpctalknext.h"
#include "pkt_npccancel.h"
#include "pkt_npccontinue.h"
#include "pkt_mapchange.h"
#include "pkt_sendnpctext.h"
#include "pkt_sendnpcnumber.h"
#include "pkt_recvnpctalkresponses.h"
#include "pkt_sendnpcresponse.h"
#include "pkt_actormove.h"
#include "pkt_actoraction.h"
#include "pkt_actionrequest.h"

#endif /* __RONET_PACKETS_PACKETS_H */
