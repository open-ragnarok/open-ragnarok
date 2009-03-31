#include "stdafx.h"

#include "ronet/packets/pkt_charlogin.h"

ronet::pktCharLogin::pktCharLogin(unsigned int acct, unsigned int sid1, unsigned int sid2, unsigned char s) : Packet(pktCharLoginID) {
	accountid = acct;
	sessionid1 = sid1;
	sessionid2 = sid2;
	sex = s;

	setSize(17);
}

bool ronet::pktCharLogin::PrepareData() {
	unsigned char* ptr = buffer;
	memcpy(ptr, (unsigned char*)&id, sizeof(short));
	ptr += sizeof(short);

	memcpy(ptr, (unsigned char*)&accountid, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, (unsigned char*)&sessionid1, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, (unsigned char*)&sessionid2, sizeof(int));
	ptr += sizeof(int);
	ptr += 2;
	*ptr = sex;
	return(true);
}
