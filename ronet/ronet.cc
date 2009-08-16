/* $Id$ */
#include "stdafx.h"

#include "ronet/ronet.h"

ronet::Packet* ronet::RONet::popPacket() {
	return(m_packetfactory.pop());
}

int ronet::RONet::packetCount() const {
	return(m_packetfactory.count());
}


bool ronet::RONet::Process() {
	m_login.Process();
	m_char.Process();
	m_map.Process();

	m_packetfactory << m_login;
	m_packetfactory << m_char;
	m_packetfactory << m_map;
	return(true);
}

ronet::TcpConnection& ronet::RONet::getLogin() {
	return(m_login);
}

ronet::TcpConnection& ronet::RONet::getChar() {
	return(m_char);
}

ronet::TcpConnection& ronet::RONet::getMap() {
	return(m_map);
}

bool ronet::RONet::GameLogin(const std::string& name, const std::string& pass) {
	if (!m_login.isConnected()) {
		std::cerr << "[RONet::GameLogin() Error] Not connected to login server" << std::endl;
		return(false);
	}

	ronet::pktLogin login(name, pass);
	login >> m_login.bufOutput;
	return(true);
}

bool ronet::RONet::CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::CharLogin() Error] Not connected to char server" << std::endl;
		return(false);
	}

	ronet::pktCharLogin login(acctid, sid1, sid2, sex);
	login >> m_char.bufOutput;
	return(true);
}

bool ronet::RONet::CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::CharLogin() Error] Not connected to char server" << std::endl;
		return(false);
	}

	ronet::pktCharCreate pkt(charname, attr, slot, color, style);
	pkt >> m_char.bufOutput;
	return(true);
}

void ronet::RONet::KeepAliveChar(unsigned int acc_id) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::KeepAliveChar() Error] Not connected to char server" << std::endl;
		return;
	}

	ronet::pktKeepAlive pkt(acc_id);
	pkt >> m_char.bufOutput;
	return;
}

void ronet::RONet::CharSelect(unsigned int slot) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::KeepAliveChar() Error] Not connected to char server" << std::endl;
		return;
	}

	ronet::pktCharSelect pkt(slot);
	pkt >> m_char.bufOutput;
	return;
}