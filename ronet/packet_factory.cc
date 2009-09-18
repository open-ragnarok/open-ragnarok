/* $Id$ */
#include "stdafx.h"

#include "ronet/packet_factory.h"

#define HANDLER(x) bool ronet::PacketFactory::Handle_ ##x (ronet::ucBuffer& b) { \
	pkt ##x *p; \
	p = new pkt ##x (); \
	if (!p->Decode(b)) { delete(p); return(false); } \
	push(p); \
	return(true); }

#define CALLER(x) m_dispatcher.Register(pkt ##x ##ID, &ronet::PacketFactory::Handle_ ##x )
// List of packets that we know how to decode

//Add new packets here
HANDLER(AttackRange);
HANDLER(GuildMessage);
HANDLER(DisplayStat);
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

ronet::PacketFactory::PacketFactory() : m_dispatcher(this) {
	//Add new packets here
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
	//m_dispatcher.Register(0x0069, &ronet::PacketFactory::Handle_ServerList);
}

ronet::PacketFactory::~PacketFactory() {
	Packet* p = pop();
	while (p != NULL) {
		delete(p);
		p = pop();
	}
}

/*
bool ronet::PacketFactory::Handle_ServerList(ronet::ucBuffer& b) {
	pktServerList *p;
	p = new pktServerList();
	if (!p->Decode(b)) {
		delete(p);
		return(false);
	}

	push(p);
	return(true);
}
*/

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
		fprintf(stderr, "Undefined call to method %04x\n", method);
		buf.clear();
		return(false);
	}
	
	Handler c = itr->second;
	return((m_factory->*c)(buf)); 
}
