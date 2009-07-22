#include "stdafx.h"

#include "ronet/packet_vector.h"

ronet::Packet* ronet::PacketVector::pop() {
	iterator itr;
	itr = begin();
	if (itr == end())
		return(NULL);
	Packet* ret = *itr;
	erase(itr);
	return(ret);
}
