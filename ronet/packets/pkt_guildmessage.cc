#include "stdafx.h"

#include "ronet/packets/pkt_guildmessage.h"

ronet::pktGuildMessage::pktGuildMessage() : Packet(pktGuildMessageID) {
}

bool ronet::pktGuildMessage::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);
	buf >> len_mes;

	memset(&text,0x00,len_mes);
	buf.read((unsigned char*)text,len_mes-4);

	return(true);
}

char *ronet::pktGuildMessage::getText(){
	return (char *)text;
}