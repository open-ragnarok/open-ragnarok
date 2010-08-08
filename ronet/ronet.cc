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

#include "ronet/ronet.h"

namespace ronet {

RONet::RONet() {
	m_packetver = 22;
}

unsigned int RONet::getPacketVer() const {
	return(m_packetver);
}

void RONet::setPacketVer(unsigned int v) {
	m_packetver = v;
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

bool RONet::GameLogin(const std::string& name, const std::string& pass, unsigned int version) {
	if (!m_login.isConnected()) {
		_log(RONET__ERROR, "GameLogin() Error: Not connected to login server");
		return(false);
	}

	// Set internal packet version for future reference.
	m_packetver = version;

	ronet::pktLogin login(name, pass, version);
	login >> m_login.bufOutput;
	return(true);
}

bool RONet::CharLogin(unsigned int acctid, unsigned int sid1, unsigned int sid2, unsigned char sex) {
	if (!m_char.isConnected()) {
		_log(RONET__ERROR, "CharLogin() Error: Not connected to char server");
		return(false);
	}

	ronet::pktCharLogin login(acctid, sid1, sid2, sex);
	login >> m_char.bufOutput;
	return(true);
}

bool RONet::CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short slot, unsigned short color, unsigned short style) {
	if (!m_char.isConnected()) {
		_log(RONET__ERROR, "[RONet::CharLogin() Error] Not connected to char server");
		return(false);
	}

	ronet::pktCharCreate pkt(charname, attr, (unsigned char)slot, color, style);
	pkt >> m_char.bufOutput;
	return(true);
}

void RONet::KeepAliveChar(unsigned int acc_id) {
	if (!m_char.isConnected()) {
		_log(RONET__ERROR, "[RONet::KeepAliveChar() Error] Not connected to char server");
		return;
	}

	ronet::pktKeepAlive pkt(acc_id);
	pkt >> m_char.bufOutput;
	return;
}

void RONet::CharSelect(unsigned int slot) {
	if (!m_char.isConnected()) {
		_log(RONET__ERROR, "[RONet::CharSelect() Error] Not connected to char server");
		return;
	}

	ronet::pktCharSelect pkt(slot);
	pkt >> m_char.bufOutput;
	return;
}

bool RONet::MapLogin(int acctid, int sid1, int sid2, unsigned int tick, int sex) {
	if (!m_map.isConnected()) {
		_log(RONET__ERROR, "[RONet::MapLogin() Error] Not connected to map server");
		return(false);
	}

	switch (m_packetver) {
		case 23:
			{
			ronet::pktMapLogin23 pkt(acctid, sid1, sid2, tick, sex);
			pkt >> m_map.bufOutput;
			break;
			}
		default:
			{
			ronet::pktMapLogin pkt(acctid, sid1, sid2, tick, sex);
			pkt >> m_map.bufOutput;
			break;
			}
	}
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

void RONet::KeepAliveMap(unsigned int acc_id) {
	if (!m_map.isConnected()) {
		_log(RONET__ERROR, "KeepAliveMap() Error: Not connected to map server");
		return;
	}

	switch (m_packetver) {
		case 23:
			{
			ronet::pktKeepAliveMap23 pkt(50, acc_id);
			pkt >> m_map.bufOutput;
			}
			break;
		default:
			{
			ronet::pktKeepAliveMap pkt(0); // ticks?
			pkt >> m_map.bufOutput;
			}
	}

	return;
}

bool RONet::Talk(unsigned int npc_id) {
	ronet::pktTalk pkt(npc_id);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::NPCNext(unsigned int id) {
	ronet::pktNpcContinue pkt(id);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::NPCCancel(unsigned int id) {
	ronet::pktNpcCancel pkt(id);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::RequestPlayerInfo(unsigned int id) {
	ronet::pktRequestPlayerInfo pkt(id);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::NPCText(unsigned int id, const char* text) {
	ronet::pktSendNpcText pkt(id, text);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::NPCNumber(unsigned int id, unsigned int num) {
	ronet::pktSendNpcNumber pkt(id, num);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::NPCResponse(unsigned int id, unsigned char num) {
	ronet::pktSendNpcResponse pkt(id, num);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::sendAction(int id, unsigned char flag) {
	ronet::pktActionRequest pkt(id, flag);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::Restart(unsigned char type) {
	ronet::pktRestart pkt(type);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::StatusUp(short type, char amount) {
	ronet::pktStatusUp pkt(type, amount);
	pkt >> m_map.bufOutput;
	return(true);
}

bool RONet::Emotion(unsigned char type) {
	ronet::pktReqEmotion pkt(type);
	pkt >> m_map.bufOutput;
	return(true);
}

//bool RONet::Quit(unsigned short type) {
bool RONet::Quit() {
//	ronet::pktQuit pkt(type);
	ronet::pktQuit pkt;
	pkt >> m_map.bufOutput;
	return(true);
}

}
