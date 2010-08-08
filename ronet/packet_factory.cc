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
HANDLER(ItemGained)
HANDLER(ItemLost)
HANDLER(InventoryItems)
HANDLER(InventoryItemsStackable)
//HANDLER(StatInfo)
//HANDLER(StatInfo2)
HANDLER(HotkeyList)
HANDLER(ZenyExp)
HANDLER(StatsInfo)
HANDLER(ActorDisplay)
HANDLER(ActorSpawn)
HANDLER(ActorWalking)
HANDLER(RecvNpcTalk)
HANDLER(RecvNpcTalkNext)
HANDLER(RecvNpcTalkClose)
HANDLER(RecvNpcInputReq)
HANDLER(RecvNpcTalkResponses)
HANDLER(RecvNpcImage2)
HANDLER(MapChange)
HANDLER(ActorMove)
HANDLER(ActorAction)
HANDLER(ActorStop)
HANDLER(RestartCharSelect)
HANDLER(StatusUpAck)
HANDLER(Emotion)
HANDLER(NotifyEffect1)
HANDLER(QuitAck)

// Guild
//HANDLER(GuildAlliesEnemiesList)
HANDLER(GuildMasterMember)
//HANDLER(GuildEmblem)
//HANDLER(GuildMembersList)
//HANDLER(GuildMemberPositionChanged)
//HANDLER(GuildLeave)
//HANDLER(GuildExpulsion)
HANDLER(GuildBroken)
//HANDLER(GuildMemberSettingList)
//HANDLER(GuildSkillsList)
//HANDLER(GuildExpulsionList)
//HANDLER(GuildMembersTitleList)
HANDLER(GuildCreateResult)
HANDLER(GuildInviteResult)
//HANDLER(GuildRequest)
//HANDLER(GuildName)
HANDLER(GuildMemberOnlineStatus)
//HANDLER(GuildNotice)
//HANDLER(GuildAllyRequest)
HANDLER(GuildAlliance)
//HANDLER(GuildPositionChanged)
HANDLER(GuildMessage)
HANDLER(GuildOppositionResult)
HANDLER(GuildUnally)
//HANDLER(GuildAllianceAdded)
//HANDLER(GuildInfo)
HANDLER(GuildLocation)
//HANDLER(GuildMemberOnlineStatus2)

// Pet
//HANDLER(PetCaptureProcess);
HANDLER(PetCaptureResult);
HANDLER(PetFood);
//HANDLER(PetInfo);
HANDLER(PetInfo2);
HANDLER(PetEmotion);
//HANDLER(EggList);


ronet::PacketFactory::PacketFactory() : m_dispatcher(this) {
	//Add new packets here
	CALLER(HpUpdateParty);
	CALLER(OtherSpeech);
	CALLER(CharLeaveScreen);
	CALLER(GmBroad);
	CALLER(ServerTick);
	CALLER(AttackRange);
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
	CALLER(ItemGained);
	CALLER(ItemLost);
	CALLER(InventoryItems);
	CALLER(InventoryItemsStackable);
	//CALLER(StatInfo);
	//CALLER(StatInfo2);
	CALLER(HotkeyList);
	CALLER(ZenyExp);
	CALLER(StatsInfo);
	CALLER(ActorDisplay);
	CALLER(ActorSpawn);
	CALLER(ActorWalking);
	CALLER(RecvNpcTalk);
	CALLER(RecvNpcTalkNext);
	CALLER(RecvNpcTalkClose);
	CALLER(RecvNpcInputReq);
	CALLER(RecvNpcTalkResponses);
	CALLER(RecvNpcImage2);
	CALLER(MapChange);
	CALLER(ActorMove);
	CALLER(ActorAction);
	CALLER(ActorStop);
	CALLER(RestartCharSelect);
	CALLER(StatusUpAck);
	CALLER(Emotion);
	CALLER(NotifyEffect1);
	CALLER(QuitAck);
	
	// Guild
	//CALLER(GuildAlliesEnemiesList);
	CALLER(GuildMasterMember);
	//CALLER(GuildEmblem);
	//CALLER(GuildMembersList);
	//CALLER(GuildMemberPositionChanged);
	//CALLER(GuildLeave);
	//CALLER(GuildExpulsion);
	CALLER(GuildBroken);
	//CALLER(GuildMemberSettingList);
	//CALLER(GuildSkillsList);
	//CALLER(GuildExpulsionList);
	//CALLER(GuildMembersTitleList);
	CALLER(GuildCreateResult);
	CALLER(GuildInviteResult);
	//CALLER(GuildRequest);
	//CALLER(GuildName);
	CALLER(GuildMemberOnlineStatus);
	//CALLER(GuildNotice);
	//CALLER(GuildAllyRequest);
	CALLER(GuildAlliance);
	//CALLER(GuildPositionChanged);
	CALLER(GuildMessage);
	CALLER(GuildOppositionResult);
	CALLER(GuildUnally);
	//CALLER(GuildAllianceAdded);
	//CALLER(GuildInfo);
	CALLER(GuildLocation);
	//CALLER(GuildMemberOnlineStatus2);

	// Pet
	//CALLER(PetCaptureProcess);
	CALLER(PetCaptureResult);
	CALLER(PetFood);
	//CALLER(PetInfo);
	CALLER(PetInfo2);
	CALLER(PetEmotion);
	//CALLER(EggList);
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
		_hexlog(RONET__ERROR, buf.getBuffer(), buf.dataSize());
		buf.clear();
		return(false);
	}
	
	Handler c = itr->second;
	return((m_factory->*c)(buf)); 
}
