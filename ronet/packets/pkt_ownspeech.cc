#include "stdafx.h"

#include "ronet/packets/pkt_ownspeech.h"

ronet::pktOwnSpeech::pktOwnSpeech() : Packet(pktOwnSpeechID) {
}

bool ronet::pktOwnSpeech::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}

	buf.ignore(2);
	buf >> len_mes;

	//if (buf.dataSize() != len_mes)
	//	return(false);

	buf.read((unsigned char*)text,len_mes-4);

	return(true);
}

char *ronet::pktOwnSpeech::getText(){
	return (char *)text;
}