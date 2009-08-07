/* $Id$ */
#include "stdafx.h"

#include "openro.h"

OpenRO::OpenRO() : ROEngine() {
	ReadIni("data.ini");
	m_state = ST_Login;
	//m_showui = true;

	m_serverlist = NULL;
}

OpenRO::~OpenRO() {
	if (m_serverlist != NULL)
		delete(m_serverlist);
}

void OpenRO::LoginScreen() {
	dskLogin->setEnabled(true);
	dskLogin->clear();
	m_state = ST_Login;
	m_gui.setDesktop(dskLogin);
	m_network.getLogin().Close();
}

void OpenRO::ProcessLogin(const std::string& user, const std::string& pass) {
	m_network.getLogin().Connect(OpenRO::ConnectionIP, OpenRO::ConnectionPort);
	m_network.GameLogin(user, pass);
	dskLogin->setEnabled(false);
}

void OpenRO::ServiceSelect(unsigned int serviceid) {
	if (serviceid < m_serverlist->getServerCount())
		return;

	m_network.getChar().Connect(m_serverlist->getInfo(serviceid).ip, m_serverlist->getInfo(serviceid).port);
	m_network.CharLogin(m_serverlist->getAccountId(), m_serverlist->getSessionId1(), m_serverlist->getSessionId2(), m_serverlist->getSex());
	dskService->setEnabled(false);
}

void OpenRO::HandleKeyboard() {
	if (keys[SDLK_ESCAPE])
		m_quit = true;
}

void OpenRO::Quit() {
	m_quit = true;
	m_state = ST_Unk;
}

void OpenRO::AfterDraw() {
	m_network.Process();

	ronet::Packet* pkt = m_network.popPacket();

#define HANDLEPKT(pktid, delp) case ronet::pkt ##pktid ##ID : delpkt = delp; hndl ##pktid ((ronet::pkt ##pktid *)pkt); break

	bool delpkt; // if it's true, we dispose of the packet
	while (pkt != NULL) {
		delpkt = true;
		switch(pkt->getID()) {
			/*
			case ronet::pktServerListID:
				delpkt = false;
				hndlServerList((ronet::pktServerList*)pkt);
				break;
			*/
			HANDLEPKT(ServerList, false);
			HANDLEPKT(CharList, true);
			default:
				std::cerr << "Unhandled packet id " << pkt->getID() << std::endl;
		}

		if (delpkt)
			delete(pkt);
		pkt = m_network.popPacket();
	}
}

void OpenRO::BeforeRun() {
	ParseClientInfo();

	InitDisplay(800, 600, false);

	dskLogin = new DesktopLogin(this);
	dskService = new DesktopService(this);
	dskCreate = new DesktopCreate(this);
	dskChar = new DesktopChar(this);
	m_gui.setDesktop(dskLogin);
}

void OpenRO::hndlServerList(ronet::pktServerList* pkt) {
	m_serverlist = pkt;
	m_gui.setDesktop(dskService);
	dskService->setEnabled(true);
	char buf[128];

	dskService->clear();
	for (unsigned int i = 0; i < pkt->getServerCount(); i++) {
		sprintf(buf, "%s (%d)", pkt->getInfo(i).name, pkt->getInfo(i).users);
		dskService->add(buf);
	}
}

void OpenRO::hndlCharList(ronet::pktCharList* pkt) {
	int count = pkt->getCount();
	printf("Received a list of %d chars\n", count);
	m_gui.setDesktop(dskChar);
	dskChar->setEnabled(true);

	for (int i = 0; i < count; i++) {
		dskChar->addChar(pkt->getChar(i));
	}
}

void OpenRO::CreateCharWindow(unsigned int& slot) {
	m_charslot = slot;
	m_gui.setDesktop(dskCreate);
	dskCreate->setEnabled(true);
}

void OpenRO::CreateChar(const std::string& charname, const CharAttributes& attr, unsigned short color, unsigned short style) {
	m_network.CreateChar(charname, attr, m_charslot, color, style);
}

void OpenRO::ParseClientInfo(){
	FileManager& fm = OpenRO::getFileManager();

	TiXmlDocument doc;
	FileData data;

	std::string name = "sclientinfo.xml";

	data = fm.getFile(name);
	if (data.blobSize() == 0) {
		std::cerr << "OpenRO::ParseClientInfo(): Error loading file " << name << std::endl;
	}
	doc.Parse((const char*)data.getBuffer());

	TiXmlElement* sclient_root = doc.FirstChildElement("clientinfo");
	if(sclient_root){
		TiXmlElement* sclient_child = sclient_root->FirstChildElement("connection");
		if(sclient_child){
			TiXmlElement* sclient_child2 = sclient_child->FirstChildElement("address");
			if(sclient_child2){
				const char *ip = sclient_child2->GetText();
				strcpy(OpenRO::ConnectionIP, ip);
			}
			free(sclient_child2);
			sclient_child2 = sclient_child->FirstChildElement("port");
			if(sclient_child2){
				const char *port = sclient_child2->GetText();
				OpenRO::ConnectionPort = atoi(port);
			}

		}
	}
}
