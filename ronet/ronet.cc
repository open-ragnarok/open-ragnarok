/* $Id$ */
#include "stdafx.h"

#include "ronet/ronet.h"

namespace ronet {

void hexdump(const unsigned char* buf, unsigned int buflen) {
	unsigned int pos = 0;
	unsigned int tbufpos = 0;
	char tbuf[32];
	char c;

	tbuf[0] = 0;

	for (pos = 0; pos < buflen; pos++) {
		if (pos % 16 == 0) {
			tbuf[tbufpos] = 0;
			if (pos > 0) {
				printf("| %s\n", tbuf);
			}
			printf("%04x | ", pos);
			tbufpos = 0;
		}
		else if (pos % 8 == 0) {
			tbuf[tbufpos++] = ' ';
			printf(" ");
		}
		printf("%02x ", buf[pos]);
		c = buf[pos];
		if (c < ' ' || c > 'z')
			c = '.';

		tbuf[tbufpos++] = c;
	}
	int rest = 16 - (pos % 16);
	if (rest >= 8)
		printf(" ");
	for (int i = 0; i < rest; i++)
		printf("   ");
	tbuf[tbufpos] = 0;
	printf("| %s\n", tbuf);
}

Packet* RONet::popPacket() {
	return(m_packetfactory.pop());
}

int RONet::packetCount() const {
	return(m_packetfactory.count());
}

bool RONet::Process() {
	m_login.Process();
	m_char.Process();
	m_map.Process();

	m_packetfactory << m_login;
	m_packetfactory << m_char;
	m_packetfactory << m_map;
	return(true);
}

TcpConnection& RONet::getLogin() {
	return(m_login);
}

TcpConnection& RONet::getChar() {
	return(m_char);
}

TcpConnection& RONet::getMap() {
	return(m_map);
}

bool RONet::GameLogin(const std::string& name, const std::string& pass) {
	if (!m_login.isConnected()) {
		std::cerr << "[RONet::GameLogin() Error] Not connected to login server" << std::endl;
		return(false);
	}

	ronet::pktLogin login(name, pass);
	login >> m_login.bufOutput;
	return(true);
}

bool RONet::CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::CharLogin() Error] Not connected to char server" << std::endl;
		return(false);
	}

	ronet::pktCharLogin login(acctid, sid1, sid2, sex);
	login >> m_char.bufOutput;
	return(true);
}

bool RONet::CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::CharLogin() Error] Not connected to char server" << std::endl;
		return(false);
	}

	ronet::pktCharCreate pkt(charname, attr, (unsigned char)slot, color, style);
	pkt >> m_char.bufOutput;
	return(true);
}

void RONet::KeepAliveChar(unsigned int acc_id) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::KeepAliveChar() Error] Not connected to char server" << std::endl;
		return;
	}

	ronet::pktKeepAlive pkt(acc_id);
	pkt >> m_char.bufOutput;
	return;
}

void RONet::CharSelect(unsigned int slot) {
	if (!m_char.isConnected()) {
		std::cerr << "[RONet::CharSelect() Error] Not connected to char server" << std::endl;
		return;
	}

	ronet::pktCharSelect pkt(slot);
	pkt >> m_char.bufOutput;
	return;
}

bool RONet::MapLogin(int acctid, int sid1, int sid2, unsigned int tick, int sex) {
	if (!m_map.isConnected()) {
		std::cerr << "[RONet::MapLogin() Error] Not connected to map server" << std::endl;
		return(false);
	}

	ronet::pktMapLogin pkt(acctid, sid1, sid2, tick, sex);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::MoveCharacter(int x, int y) {
	if (!m_map.isConnected())
		return(false);

	ronet::pktMapMove pkt(x, y);
	pkt >> m_map.bufOutput;

	return(true);
}

bool RONet::RequestGuiildInfo() {
	if (!m_map.isConnected())
		return(false);

	ronet::pktGuildInfoRequest pkt;
	pkt >> m_map.bufOutput;

	return(true);
}

bool RONet::MapLoaded() {
	if (!m_map.isConnected())
		return(false);

	ronet::pktMapLoaded pkt;
	pkt >> m_map.bufOutput;

	return(true);
}


}
