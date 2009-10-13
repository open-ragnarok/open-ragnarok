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

#include "ronet/packet_factory.h"

#define HANDLER(x) \
bool ronet::PacketFactory::Handle_ ##x (ronet::ucBuffer& b) { \
	pkt ##x *p; \
	p = new pkt ##x (); \
	if (!p->Decode(b)) { delete(p); return(false); } \
	push(p); \
	return(true); \
}

#define CALLER(x) \
	m_dispatcher.Register(pkt ##x ##ID, &ronet::PacketFactory::Handle_ ##x )

//Add new packets here
HANDLER(HpUpdateParty)
HANDLER(OtherSpeech)
HANDLER(CharLeaveScreen)
HANDLER(GmBroad)
HANDLER(ServerTick)
HANDLER(AttackRange)
HANDLER(GuildMessage)
HANDLER(DisplayStat)
HANDLER(UpdateStatus)
HANDLER(ServerList)
HANDLER(CharList)
HANDLER(CharCreateError)
HANDLER(LoginError)
HANDLER(AuthFailed)
HANDLER(CharCreated)
HANDLER(CharPosition)
HANDLER(MapAcctSend)
HANDLER(MapLoginSuccess)
HANDLER(OwnSpeech)
HANDLER(SkillList)
HANDLER(MapMoveOk)
HANDLER(PlayerEquip)
HANDLER(InventoryItems)
HANDLER(StatInfo)
//HANDLER(StatInfo2)
HANDLER(HotkeyList)
HANDLER(ZenyExp)
HANDLER(StatsInfo)
HANDLER(ActorDisplay)
HANDLER(RecvNpcTalk)
HANDLER(RecvNpcTalkNext)
HANDLER(RecvNpcTalkClose)
HANDLER(RecvNpcInputReq)
HANDLER(RecvNpcTalkResponses)
HANDLER(MapChange)

ronet::PacketFactory::PacketFactory() : m_dispatcher(this) {
	//Add new packets here
	CALLER(HpUpdateParty);
	CALLER(OtherSpeech);
	CALLER(CharLeaveScreen);
	CALLER(GmBroad);
	CALLER(ServerTick);
	CALLER(AttackRange);
	CALLER(GuildMessage);
	CALLER(DisplayStat);
	CALLER(UpdateStatus);
	CALLER(ServerList);
	CALLER(CharList);
	CALLER(CharCreateError);
	CALLER(LoginError);
	CALLER(AuthFailed);
	CALLER(CharCreated);
	CALLER(CharPosition);
	CALLER(MapAcctSend);
	CALLER(MapLoginSuccess);
	CALLER(OwnSpeech);
	CALLER(SkillList);
	CALLER(MapMoveOk);
	CALLER(PlayerEquip);
	CALLER(InventoryItems);
	CALLER(StatInfo);
	//CALLER(StatInfo2);
	CALLER(HotkeyList);
	CALLER(ZenyExp);
	CALLER(StatsInfo);
	CALLER(ActorDisplay);
	CALLER(RecvNpcTalk);
	CALLER(RecvNpcTalkNext);
	CALLER(RecvNpcTalkClose);
	CALLER(RecvNpcInputReq);
	CALLER(RecvNpcTalkResponses);
	CALLER(MapChange);
}

ronet::PacketFactory::~PacketFactory() {
	Packet* p = pop();
	while (p != NULL) {
		delete(p);
		p = pop();
	}
}

void ronet::PacketFactory::push(ronet::Packet* p) {
	packets.push_back(p);
}

ronet::Packet* ronet::PacketFactory::pop() {
	std::vector<Packet*>::iterator itr;
	itr = packets.begin();
	if (itr == packets.end())
		return(NULL);
	Packet* ret = *itr;
	packets.erase(itr);
	return(ret);
}

unsigned int ronet::PacketFactory::count() const {
	return(packets.size());
}

void ronet::PacketFactory::generatePackets(ucBuffer& buffer) {
	unsigned short id;
	unsigned short id2;
	while (buffer.dataSize() > 0) {
		//TODO: Re-Fix this! (Temporal fix)
		//If we receive a packet id > 0x0500
		//we will ignore the first 4 bytes of the buffer
		buffer.peek((unsigned char*)&id2, 2);
		if(id2 > 0x0500){
			if(buffer.dataSize() == 4){
				buffer.clear();
				return;
			}else{
				buffer.ignore(4);
			}
		}

		buffer.peek((unsigned char*)&id, sizeof(short));
		if(!m_dispatcher.Call(id, buffer))
			return;
	}
}

ronet::PacketFactory& ronet::PacketFactory::operator << (ucBuffer& buffer) {
	generatePackets(buffer);
	return(*this);
}

ronet::PacketFactory& ronet::PacketFactory::operator << (Connection& c) {
	generatePackets(c.bufInput);
	return(*this);
}

ronet::PacketFactory::Dispatcher::Dispatcher(ronet::PacketFactory* f) {
	m_factory = f;
}

void ronet::PacketFactory::Dispatcher::Register(unsigned short id, ronet::PacketFactory::Handler call) {
	m_calls[id] = call;
}

bool ronet::PacketFactory::Dispatcher::Call(unsigned short method, ucBuffer& buf) {
	std::map<unsigned short, Handler>::iterator itr;
	itr = m_calls.find(method);
	if (itr == m_calls.end()) {
		_log(RONET__ERROR, "Undefined call to method %04x", method);
		buf.clear();
		return(false);
	}
	
	Handler c = itr->second;
	return((m_factory->*c)(buf)); 
}
