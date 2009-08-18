#include "stdafx.h"

#include "ronet/packets/pkt_mapacctsend.h"

ronet::pktMapAcctSend::pktMapAcctSend(int acct) : Packet(pktMapAcctSendID) {
	account_id = acct;
	setSize(6);
}

bool ronet::pktMapAcctSend::Decode(ucBuffer& buf) {
	// Sanity check
	unsigned short buf_id;
	buf.peek((unsigned char*)&buf_id, 2);
	if (buf_id != id) {
		fprintf(stderr, "Wrong packet id! (%04x != %04x)\n", id, buf_id);
		return(false);
	}
	if (buf.dataSize() < 6)
		return(false);

	buf.ignore(2);
	buf >> account_id;

	return(true);
}

int ronet::pktMapAcctSend::getAccountId() const{
	return account_id;
}