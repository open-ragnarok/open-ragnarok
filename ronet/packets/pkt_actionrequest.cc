#include "stdafx.h"

#include "ronet/packets/pkt_actionrequest.h"


namespace ronet {

pktActionRequest::pktActionRequest(unsigned int target, unsigned char flag) : Packet(pktActionRequestID) {
//	setSize(18);
	setSize(7);
	m_target = target;
	m_flag = flag;
}

bool pktActionRequest::PrepareData() {
	unsigned char* ptr = buffer;

#if 1 // Packet ver 5
	memcpy(ptr, (unsigned char*)&pktID, sizeof(short));
	ptr += sizeof(short);

	memcpy((void*)(ptr), (void*)&m_target, 4);
	ptr += 4;

	*ptr = m_flag;
#else // Packet ver 20
//	pktActionRequestID = 0x0190,	// <packet_id>,<packet_len>,actionrequest,<offset of target_id (4 bytes)>,<offset of action_type (1 byte)> -- Thanks, FlavioJS!
	memset(ptr, 0, 18);

	memcpy(ptr, (unsigned char*)&pktID, sizeof(short));
	ptr += sizeof(short);

//	*(ptr++) = 18; // Packet len
	unsigned short packet_len = 18;
	memcpy(ptr, (unsigned char*)&packet_len, sizeof(short));
	ptr += sizeof(short);


	int offset = 7;
	memcpy((void*)ptr, (void*)&offset, 4); // offset target_id
	ptr += 4;
/*	unsigned short offset = 7;
	memcpy(ptr, (unsigned char*)&offset, sizeof(short));
	ptr += sizeof(short);*/

//	*(ptr++) = 6; // offset target_id
//	ptr++;

	*(ptr++) = 11; // offset action_type
	ptr++;

//	memcpy((void*)(ptr+6), (void*)&m_target, 4);
//	ptr[16] = m_flag;
	memcpy((void*)(ptr), (void*)&m_target, 4);
	ptr += 4;

//	ptr[6] = m_flag;
	*ptr = m_flag;

//	ptr++;

//	_hexlog(RONET__DEBUG, ptr, 18);
#endif

	return(true);
}

}
