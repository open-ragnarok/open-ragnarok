#include "stdafx.h"

#include "ronet/packets/pkt_actionrequest.h"


namespace ronet {

pktActionRequest::pktActionRequest(unsigned int target, unsigned char flag) : Packet(pktActionRequestID) {
	setSize(18);
	m_target = target;
	m_flag = flag;
}

bool pktActionRequest::PrepareData() {
	unsigned char* ptr = buffer;

	memcpy((void*)(ptr+6), (void*)&m_target, 4);
	ptr[16] = m_flag;

	_hexlog(RONET__DEBUG, ptr, 18);

	return(true);
}

}
